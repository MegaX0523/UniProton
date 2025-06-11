/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2023. All rights reserved.
 */

#ifndef __MCS_H__
#define __MCS_H__

#include "bm_types.h"
#include "hi309x_memmap.h"

#define VDEV_START_ADDR MMU_OPENAMP_ADDR
#define VDEV_SIZE MMU_OPENAMP_ADDR_SIZE

#define VDEV_STATUS_ADDR VDEV_START_ADDR
#define VDEV_STATUS_SIZE 0x4000

#define SHARE_MEM_START_ADDR (VDEV_START_ADDR + VDEV_STATUS_SIZE)
#define SHARE_MEM_SIZE (VDEV_SIZE - VDEV_STATUS_SIZE)
#define SHARE_MEM_DEVICE_NAME "lonely_device"

#define VRING_COUNT 2
#define VRING_RX_ADDRESS (VDEV_START_ADDR + SHARE_MEM_SIZE - VDEV_STATUS_SIZE)
#define VRING_TX_ADDRESS (VDEV_START_ADDR + SHARE_MEM_SIZE)
#define VRING_ALIGNMENT 4
#define VRING_SIZE 16

#define CONFIG_RPMSG_SERVICE_NUM_ENDPOINTS 1

#define DEFAULT_PAGE_SHIFT 0xffffffffffffffffULL
#define DEFAULT_PAGE_MASK 0xffffffffffffffffULL

#define VIRTQUEUE_ID 1
#define RPMSG_ROLE RPMSG_REMOTE

#define OS_OPENAMP_NOTIFY_HWI_NUM 7
#define OS_OPENAMP_NOTIFY_HWI_PRIO 0

#define CHANNEL_FREE 0
#define CHANNEL_USED 1

inline void sys_write32(unsigned int data, uintptr_t addr)
{
    __asm__ volatile("dmb sy" : : : "memory");
    __asm__ volatile("str %w0, [%1]" : : "r"(data), "r"(addr));
}

inline unsigned int sys_read32(uintptr_t addr)
{
    unsigned int val;
    __asm__ volatile("ldr %w0, [%1]" : "=r"(val) : "r"(addr));
    __asm__ volatile("dmb sy" : : : "memory");
    return val;
}

inline void sys_write8(unsigned char data, uintptr_t addr)
{
    __asm__ volatile("dmb sy" : : : "memory");
    __asm__ volatile("strb %w0, [%1]" : : "r"(data), "r"(addr));
}

inline unsigned char sys_read8(uintptr_t addr)
{
    unsigned char val;
    __asm__ volatile("ldrb %w0, [%1]" : "=r"(val) : "r"(addr));
    __asm__ volatile("dmb sy" : : : "memory");
    return val;
}

#endif /* __MCS_H__ */
