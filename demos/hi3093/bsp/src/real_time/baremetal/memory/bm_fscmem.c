/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2023. All rights reserved.
 */
#include "bm_mem_common.h"
#include "bm_mem.h"
#include "securec.h"

struct tag_memory_funcs g_memory_arith_api; /* 算法对应API */
struct tag_memory_ctrl_fsc g_memory_node_list[FSC_MEM_LAST_IDX];
unsigned int g_memory_bit_amp = 1;
unsigned char g_memory_region[MEM_FSC_PT_SIZE];

/*
 * 模块内内联函数定义
 */
SEC_ALW_INLINE INLINE void bm_memory_delete_fsc(struct tag_memory_ctrl_fsc *current_block)
{
    current_block->next->prev = current_block->prev;
    current_block->prev->next = current_block->next;
}

SEC_ALW_INLINE INLINE void bm_memory_insert_fsc(struct tag_memory_ctrl_fsc *current_block,
                                                struct tag_memory_ctrl_fsc *fsc_free_list, unsigned int *bit_map_ptr)
{
    unsigned int idx = FSC_MEM_SZ2IDX(current_block->size);
    struct tag_memory_ctrl_fsc *head_block = &(fsc_free_list[idx]);

    *bit_map_ptr |= FSC_MEM_IDX2BIT(idx);

    current_block->prev = head_block;
    current_block->next = head_block->next;
    head_block->next->prev = current_block;
    head_block->next = current_block;
}

/* 对齐之后，返回地址不一定紧跟在内存头后面，需要设置返回地址与内存头之间的差值 */
SEC_ALW_INLINE INLINE void bm_memory_set_head_addr(uintptr_t use_addrr, uintptr_t ctrl_addr)
{
    unsigned short *head_addr = (unsigned short *)(use_addrr) - 1;

    *head_addr  = (unsigned short)(use_addrr - ctrl_addr);
    return;
}

SEC_ALW_INLINE INLINE void *bm_memory_get_head_addr(uintptr_t use_addrr)
{
    unsigned short head_offset = *((unsigned short *)use_addrr - 1);

    return (void *)(uintptr_t)((use_addrr - (uintptr_t)head_offset) - sizeof(struct tag_memory_ctrl_fsc));
}

static struct tag_memory_ctrl_fsc *search_useable_fsc(struct tag_memory_ctrl_fsc *current_block, unsigned int size,
                                                      struct tag_memory_ctrl_fsc *head_block, unsigned int *idx)
{
    head_block = &g_memory_node_list[*idx];
    current_block = head_block->next;

    /* 空闲链表非空 */
    while (current_block != head_block) {
        if ((current_block->size) >= size) {
            return current_block;
        }
        current_block = current_block->next;
    }

    return NULL;
}

static struct tag_memory_ctrl_fsc *bm_memory_search_fsc(unsigned int size, unsigned int *idx)
{
    unsigned int state_idx;
    struct tag_memory_ctrl_fsc *current_block = NULL;
    struct tag_memory_ctrl_fsc *head_block = NULL;
    
    state_idx = FSC_MEM_SZ2IDX(size);
    *idx = state_idx + 1;

    while (TRUE) {
        *idx = bm_get_bits((g_memory_bit_amp << *idx) >> *idx);
        if ((*idx) >= FSC_MEM_LAST_IDX) {
            *idx = state_idx;

            current_block = search_useable_fsc(current_block, size, head_block, idx);
            return current_block;
        }

        head_block = &g_memory_node_list[*idx];
        /* 空闲链表为空，清除BitMap标志位 */
        if (head_block->next == head_block) {
            g_memory_bit_amp &= ~(FSC_MEM_IDX2BIT(*idx));
        } else {
            break;
        }
    }
    current_block = head_block->next;

    return current_block;
}

static void *bm_memory_alloc_inner_fs(unsigned int size, uintptr_t align)
{
    unsigned int idx;
    unsigned int alloc_size;
    unsigned int *block_tail_magic = NULL;
    uintptr_t use_addrr;
    struct tag_memory_ctrl_fsc *plot_block = NULL;
    struct tag_memory_ctrl_fsc *current_block = NULL;
    struct tag_memory_ctrl_fsc *next_block = NULL;

    if (size == 0) {
        return NULL;
    }

    /* 由于已经按FSC_MEM_SIZE_ALIGN字节对齐，最大可能补齐的大小是align - FSC_MEM_SIZE_ALIGN */
    alloc_size = ALIGN(size, FSC_MEM_SIZE_ALIGN) + (align - FSC_MEM_SIZE_ALIGN) +
        FSC_MEM_USED_HEAD_SIZE + FSC_MEM_TAIL_SIZE;
    if ((alloc_size < size) || alloc_size >= ((FSC_MEM_MAXVAL - FSC_MEM_USED_HEAD_SIZE) - FSC_MEM_TAIL_SIZE)) {
        return NULL;
    }

    current_block = bm_memory_search_fsc(alloc_size, &idx);
    if (current_block == NULL) {
        return NULL;
    }

    /* 找到足够空间的空闲链表，并对其进行分割 */
    if ((current_block->size) >= (alloc_size + FSC_MEM_MIN_SIZE)) {
        current_block->size -= alloc_size;

        /* 调整链表 */
        if (idx != FSC_MEM_SZ2IDX(current_block->size)) {
            bm_memory_delete_fsc(current_block);
            bm_memory_insert_fsc(current_block, g_memory_node_list, &g_memory_bit_amp);
        }

        plot_block = (struct tag_memory_ctrl_fsc *)((uintptr_t)current_block + (uintptr_t)current_block->size);
        plot_block->prev_size = current_block->size;
        plot_block->size = alloc_size;

        current_block = plot_block;
    } else {
        bm_memory_delete_fsc(current_block);
    }

    next_block = (struct tag_memory_ctrl_fsc *)((uintptr_t)current_block + (uintptr_t)current_block->size);
    next_block->prev_size = 0;
    current_block->next = FSC_MEM_MAGIC_USED;

    /* 设置内存越界检查魔术字 */
    uintptr_t block_size_fsc = (uintptr_t)(current_block->size) - (uintptr_t)(FSC_MEM_TAIL_SIZE);
    block_tail_magic = (unsigned int *)((uintptr_t)current_block + block_size_fsc);
    *block_tail_magic = FSC_MEM_TAIL_MAGIC;

    // current_block->prev 复用为内存对齐的偏移地址
    current_block->prev = 0;
    use_addrr = (((uintptr_t)current_block + FSC_MEM_SLICE_HEAD_SIZE + align - 1) & ~(align - 1));
    bm_memory_set_head_addr(use_addrr, ((uintptr_t)current_block + FSC_MEM_SLICE_HEAD_SIZE));

    return (void *)use_addrr;
}

static unsigned int bm_memory_free_fsc(void *addr)
{
    struct tag_memory_ctrl_fsc *prev_block = NULL; /* 前一内存块指针 */
    struct tag_memory_ctrl_fsc *current_block = NULL; /* 当前内存块指针 */
    struct tag_memory_ctrl_fsc *next_block = NULL; /* 后一内存块指针 */
    unsigned int *block_tail_magic = NULL;
    uintptr_t block_size;

    if (addr == NULL) {
        return ERRNO_MEM_FREE_ADDR_INVALID;
    }

    current_block = (struct tag_memory_ctrl_fsc *)bm_memory_get_head_addr((uintptr_t)addr);
    block_size = current_block->size;

    if ((current_block->next != FSC_MEM_MAGIC_USED) || ((current_block->size) == 0)) {
        return ERRNO_MEM_FREE_SH_DAMAGED;
    }

    block_tail_magic = (unsigned int *)((uintptr_t)current_block + block_size - (uintptr_t)FSC_MEM_TAIL_SIZE);
    if (*block_tail_magic != FSC_MEM_TAIL_MAGIC) {
        return ERRNO_MEM_OVERWRITE;
    }

    next_block = (struct tag_memory_ctrl_fsc *)((uintptr_t)current_block + block_size);

    /* 后一内存块未使用，当前模块释放后与其合并 */
    if (next_block->next != FSC_MEM_MAGIC_USED) {
        bm_memory_delete_fsc(next_block);

        current_block->size += next_block->size;

        if (memset_s(next_block, sizeof(struct tag_memory_ctrl_fsc), 0, sizeof(struct tag_memory_ctrl_fsc)) != EOK) {
            return HW_FAIL;
        }
    }

    /* 前一内存块未使用，当前内存模块与其合并 */
    if (current_block->prev_size != 0) {
        prev_block = (struct tag_memory_ctrl_fsc *)((uintptr_t)current_block - (uintptr_t)current_block->prev_size);
        prev_block->size += current_block->size;

        bm_memory_delete_fsc(prev_block);

        if (memset_s(current_block, sizeof(struct tag_memory_ctrl_fsc), 0, sizeof(struct tag_memory_ctrl_fsc)) != EOK) {
            return HW_FAIL;
        }
      
        current_block = prev_block;
    }

    /* 合并后的总内存块插入链表 */
    bm_memory_insert_fsc(current_block, g_memory_node_list, &g_memory_bit_amp);
    next_block = (struct tag_memory_ctrl_fsc *)((uintptr_t)current_block + (uintptr_t)current_block->size);
    next_block->prev_size = current_block->size;

    return EOK;
}

static void *bm_memory_alloc_os(unsigned int size)
{
    return bm_memory_alloc_inner_fs(size, FSC_MEM_SIZE_ALIGN);
}

static void *bm_memory_alloc_os_align(unsigned int size, enum MemAlign align_pow)
{
    if (align_pow >= MEM_ADDR_BUTT || align_pow < MEM_ADDR_ALIGN_004) {
        return NULL;
    }
    return bm_memory_alloc_inner_fs(size, (1U << (unsigned int)align_pow));
}

/*
 * 描述：初始化内存
 */
unsigned int bm_memory_init_fsc(uintptr_t addr, unsigned int size)
{
    unsigned int idx;
    struct tag_memory_ctrl_fsc *head_block = NULL;
    struct tag_memory_ctrl_fsc *current_block = NULL;
    struct tag_memory_ctrl_fsc *next_block = NULL;

    /* 异常判断 */
    if ((void *)(uintptr_t)addr == NULL) {
        return ERRNO_MEM_INITADDR_ISINVALID;
    }

    if (MEM_GETBIT(addr) != 0U) {
        return ERRNO_MEM_INITADDR_INVALID;
    }

    if (MEM_GETBIT(size) != 0U) {
        return ERRNO_MEM_INITSIZE_INVALID;
    }

    if (size < FSC_MEM_USED_HEAD_SIZE) {
        return ERRNO_MEM_PTCREATE_SIZE_ISTOHWMALL;
    }

    if (size > FSC_MEM_MAXVAL) {
        return ERRNO_MEM_PTCREATE_SIZE_ISTOOBIG;
    }

    if (memset_s((void *)(uintptr_t)addr, size, 0, size) != EOK) {
        return HW_FAIL;
    }
   
    /* 链表初始化，指向自己 */
    head_block = &g_memory_node_list[0];
    for (idx = 0; idx < FSC_MEM_LAST_IDX; idx++, head_block++) {
        head_block->prev = head_block;
        head_block->next = head_block;
    }

    size -= FSC_MEM_USED_HEAD_SIZE;
    unsigned int flag_bits = 31;
    g_memory_bit_amp |= 1U << (flag_bits - FSC_MEM_LAST_IDX);

    /* 获取索引号 */
    idx = FSC_MEM_SZ2IDX(size);
    g_memory_bit_amp |= FSC_MEM_IDX2BIT(idx);

    /* 挂载链表初始化 */
    head_block = &g_memory_node_list[idx];
    current_block = (struct tag_memory_ctrl_fsc *)(uintptr_t)addr;
    current_block->next = head_block;
    current_block->prev_size = 0;
    current_block->size = size;
    current_block->prev = head_block;
    head_block->next = current_block;
    head_block->prev = current_block;

    next_block = (struct tag_memory_ctrl_fsc *)((uintptr_t)current_block + (uintptr_t)(current_block->size));
    next_block->next = FSC_MEM_MAGIC_USED;
    next_block->size = 0;

    g_memory_arith_api.alloc = bm_memory_alloc_os;
    g_memory_arith_api.alloc_align = bm_memory_alloc_os_align;
    g_memory_arith_api.free = bm_memory_free_fsc;

    return EOK;
}

unsigned int bm_init_hwi(void)
{
    unsigned int ret = bm_memory_init_fsc(MEM_FSC_PT_ADDR, MEM_FSC_PT_SIZE);

    return ret;
}