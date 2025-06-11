/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2023-2023. All rights reserved.
 */

#include "common/os_base.h"
#include "securec.h"
#include "sre_hwi.h"
#include "sre_mcs.h"
#include "sre_common.h"
#include "metal/device.h"
#include "pthread.h"
#include "os_cpu_armv8_external.h"
#include "mcs.h"

static struct virtio_device g_vdev;
static struct rpmsg_virtio_device g_rvdev;
static struct metal_io_region *g_io;
static struct rpmsg_device *g_rdev;
static SemHandle g_mcs_sem;

static metal_phys_addr_t libck_shm_physmap[] = {SHARE_MEM_START_ADDR};
static struct metal_device shm_device = {
    .name = SHARE_MEM_DEVICE_NAME,
    .bus = NULL,
    .num_regions = 1,
    {
        {
            .virt = (void *)SHARE_MEM_START_ADDR,
            .physmap = libck_shm_physmap,
            .size = SHARE_MEM_SIZE,
            .page_shift = MCS_DEFAULT_PAGE_SHIFT,
            .page_mask = MCS_DEFAULT_PAGE_MASK,
            .mem_flags = 0,
            .ops = {NULL},
        },
    },
    .node = {NULL},
    .irq_num = 0,
    .irq_info = NULL,
};

struct mcs_info {
    struct rpmsg_endpoint ept;
    rpmsg_ept_cb callback;
    int g_channel;
};

static struct mcs_info g_mcs_data[EPT_NUM + 1];

static struct virtqueue *vq[2];

static struct virtio_vring_info rvrings[2] = {
    [0] =
        {
            .info.align = VRING_ALIGNMENT_LENGTH,
        },
    [1] =
        {
            .info.align = VRING_ALIGNMENT_LENGTH,
        },
};

static void virtio_set_status(struct virtio_device *vdev, unsigned char status)
{
    (void)vdev;
    sys_write8(status, VDEV_STATUS_ADDR);
}

static unsigned char virtio_get_status(struct virtio_device *vdev)
{
    (void)vdev;
    return sys_read8(VDEV_STATUS_ADDR);
}

static void virtio_set_features(struct virtio_device *vdev, unsigned int features)
{
    (void)vdev;
    (void)features;
}

static unsigned int virtio_get_features(struct virtio_device *vdev)
{
    (void)vdev;
    return (1 << VIRTIO_RPMSG_F_NS);
}

static void virtio_notify(struct virtqueue *vque)
{
    (void)vque;
    unsigned short core_mask = 1;

    OsHwiMcTrigger(core_mask, OS_HWI_IPI_NO_07);
}

const struct virtio_dispatch vir_dispatch = {
    .get_features = virtio_get_features,
    .set_features = virtio_set_features,
    .get_status = virtio_get_status,
    .set_status = virtio_set_status,
    .notify = virtio_notify,
};

static void rpmsg_hwi_handler(void *arg)
{
    (void)arg;
    virtqueue_notification(vq[VIRTQUEUE_ID]);
}

static unsigned int rpmsg_hwi_init(void)
{
    unsigned int ret;

    ret = SRE_HwiSetAttr(OS_OPENAMP_NOTIFY_HWI_NUM, OS_OPENAMP_NOTIFY_HWI_PRIO, OS_HWI_MODE_ENGROSS);
    if (ret != OS_OK) {
        return ret;
    }

    ret = SRE_HwiCreate(OS_OPENAMP_NOTIFY_HWI_NUM, (HwiProcFunc)rpmsg_hwi_handler, 0);
    if (ret != OS_OK) {
        return ret;
    }

    ret = SRE_HwiEnable(OS_OPENAMP_NOTIFY_HWI_NUM);
    if (ret != OS_OK) {
        return ret;
    }

    return OS_OK;
}

static int rpmsg_backend_init(struct metal_io_region **io, struct virtio_device *vdev)
{
    int err;
    struct metal_init_params metal_params = METAL_INIT_DEFAULTS;
    struct metal_device *device;

    err = rpmsg_hwi_init();
    if (err) {
        return err;
    }

    /* Libmetal setup */
    err = metal_init(&metal_params);
    if (err) {
        return err;
    }

    err = metal_register_generic_device(&shm_device);
    if (err) {
        return err;
    }

    err = metal_device_open("generic", SHARE_MEM_DEVICE_NAME, &device);
    if (err) {
        return err;
    }

    *io = metal_device_io_region(device, 0);

    if (!*io) {
        return err;
    }

    /* Virtqueue setup */

    vq[0] = virtqueue_allocate(VRING_SIZE);
    if (!vq[0]) {
        return OS_ERROR;
    }

    vq[1] = virtqueue_allocate(VRING_SIZE);
    if (!vq[1]) {
        virtqueue_free(vq[0]);
        vq[0] = NULL;
        return OS_ERROR;
    }

    rvrings[0].io = *io;
    rvrings[0].vq = vq[0];
    rvrings[0].info.vaddr = (void *)VRING_TX_ADDRESS;
    rvrings[0].info.align = VRING_ALIGNMENT_LENGTH;
    rvrings[0].info.num_descs = VRING_SIZE;

    rvrings[1].io = *io;
    rvrings[1].vq = vq[1];
    rvrings[1].info.vaddr = (void *)VRING_RX_ADDRESS;
    rvrings[1].info.align = VRING_ALIGNMENT_LENGTH;
    rvrings[1].info.num_descs = VRING_SIZE;

    vdev->role = RPMSG_ROLE;
    vdev->vrings_num = VRING_COUNT;
    vdev->func = &vir_dispatch;
    vdev->vrings_info = &rvrings[0];

    return 0;
}

static struct rpmsg_endpoint *get_rpmsg_endpoint(const char *name)
{
    struct rpmsg_endpoint *ept = NULL;
    if (name == NULL) {
        return ept;
    }

    for (int i = 1; i < (EPT_NUM + 1); ++i) {
        if (strcmp(name, g_mcs_data[i].ept.name) == 0) {
            return &(g_mcs_data[i].ept);
        }
    }

    return ept;
}

int mcs_deinit(void);
int mcs_deinit(void)
{
    SRE_SemDelete(g_mcs_sem);
    rpmsg_deinit_vdev(&g_rvdev);
    if (g_io) {
        free(g_io);
        g_io = NULL;
    }

    return OS_OK;
}

int mcs_init(void)
{
    if (OsGetCoreID() != LIBCK_CORE_ID) {
        return OS_ERROR;
    }

    if (rpmsg_backend_init(&g_io, &g_vdev)) {
        return OS_ERROR;
    }

    if (rpmsg_init_vdev(&g_rvdev, &g_vdev, NULL, g_io, NULL)) {
        return OS_ERROR;
    }

    g_rdev = rpmsg_virtio_get_rpmsg_device(&g_rvdev);

    if (SRE_SemCreate(1, &g_mcs_sem)) {
        return OS_ERROR;
    }

    return OS_OK;
}

static int mcs_find_free_channel(int *channel)
{
    if (channel == NULL) {
        return OS_ERROR;
    }

    for (int i = 1; i < EPT_NUM + 1; i++) {
        if (g_mcs_data[i].g_channel == CHANNEL_FREE) {
            g_mcs_data[i].g_channel = CHANNEL_USED;
            *channel = i;
            return OS_OK;
        }
    }
    return OS_ERROR;
}

int sre_mcs_create_ept(const char *ep_name, rpmsg_ept_cb cb)
{
    if (ep_name == NULL || cb == NULL) {
        return OS_ERROR;
    }

    int channel;
    if (SRE_SemPend(g_mcs_sem, OS_WAIT_FOREVER)) {
        return OS_ERROR;
    }

    int err = mcs_find_free_channel(&channel);
    if (err) {
        SRE_SemPost(g_mcs_sem);
        return OS_ERROR;
    }

    err = rpmsg_create_ept(&(g_mcs_data[channel].ept), g_rdev, ep_name, channel, RPMSG_ADDR_ANY, cb, rpmsg_destroy_ept);

    SRE_SemPost(g_mcs_sem);

    if (err) {
        return OS_ERROR;
    }

    return OS_OK;
}

int sre_mcs_destroy_ept(const char *ep_name)
{
    if (ep_name == NULL) {
        return OS_ERROR;
    }

    struct rpmsg_endpoint *ept = get_rpmsg_endpoint(ep_name);
    if (ept == NULL) {
        return OS_ERROR;
    }

    if (ept->addr > EPT_NUM && ept->addr <= 0) {
        return OS_ERROR;
    }

    if (SRE_SemPend(g_mcs_sem, OS_WAIT_FOREVER)) {
        return OS_ERROR;
    }

    rpmsg_destroy_ept(ept);

    if (memset_s(&(g_mcs_data[ept->addr]), sizeof(struct mcs_info), 0, sizeof(struct mcs_info)) != EOK) {
        SRE_SemPost(g_mcs_sem);
        return OS_ERROR;
    }

    SRE_SemPost(g_mcs_sem);
    return OS_OK;
}

int sre_mcs_send(const char *ep_name, const void *data, int len)
{
    if (ep_name == NULL || data == NULL || len <= 0) {
        return OS_ERROR;
    }

#if defined(BM_ADDR_PROTECT) && (BM_ADDR_PROTECT == 1)
    unsigned long long tmp = (unsigned long long)(uintptr_t)data + (unsigned long long)len;
    if (((unsigned int)(uintptr_t)data < SHARE_MEM_START_ADDR) ||
        ((unsigned int)(uintptr_t)data > (SHARE_MEM_START_ADDR + SHARE_MEM_SIZE)) || (tmp < SHARE_MEM_START_ADDR) ||
        (tmp > (SHARE_MEM_START_ADDR + SHARE_MEM_SIZE))) {
        return OS_ERROR;
    }
#endif

    struct rpmsg_endpoint *ept = get_rpmsg_endpoint(ep_name);
    if (ept == NULL) {
        return OS_ERROR;
    }

    int header_len = 16;
    if (len > (RPMSG_BUFFER_SIZE - header_len)) {
        return OS_ERROR;
    }

    return rpmsg_send_offchannel_nocopy_hwi(ept, data, len);
}

void *sre_mcs_get_zero_cpy_data_addr(const char *ep_name, int *buffer_len)
{
    if (ep_name == NULL || buffer_len == NULL) {
        return NULL;
    }

    struct rpmsg_endpoint *ept = get_rpmsg_endpoint(ep_name);
    if (ept == NULL) {
        return NULL;
    }

    int wait = 1;
    return rpmsg_get_tx_payload_buffer(ept, buffer_len, wait);
}

void sre_mcs_shutdown(void)
{
    unsigned long a0 = 0x84000002;  // 0x84000002 pwr config register
    asm volatile("mov x0, %0" ::"r"(a0));
    asm volatile("smc #0");
}