/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: 内存基本功能模块的对外头文件。
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-10-27 LibCK项目组  华为规范整改
 */
/*
 * * @defgroup SRE_mem 内存基本功能
 * @ingroup SRE_memory
 */

#ifndef SRE_MEM_H
#define SRE_MEM_H

#include "sre_base.h"
#include "./common/os_mem.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/*
 * @ingroup SRE_mem
 * 申请的内存地址对齐标准:4/8字节对齐。
 */
#define OS_MEM_ADDR_ALIGN sizeof(uintptr_t)

/*
 * @ingroup SRE_mem
 * 内存对齐方式。
 */
enum MemAlign {
    MEM_ADDR_ALIGN_004 = 2, /* 4字节对齐 */
    MEM_ADDR_ALIGN_008 = 3, /* 8字节对齐 */
    MEM_ADDR_ALIGN_016 = 4, /* 16字节对齐 */
    MEM_ADDR_ALIGN_032 = 5, /* 32字节对齐 */
    MEM_ADDR_ALIGN_064 = 6, /* 64字节对齐 */
    MEM_ADDR_ALIGN_128 = 7, /* 128字节对齐 */
    MEM_ADDR_ALIGN_256 = 8, /* 256字节对齐 */
    MEM_ADDR_ALIGN_512 = 9, /* 512字节对齐 */
    MEM_ADDR_ALIGN_1K = 10, /* 1K字节对齐 */
    MEM_ADDR_ALIGN_2K = 11, /* 2K字节对齐 */
    MEM_ADDR_ALIGN_4K = 12, /* 4K字节对齐 */
    MEM_ADDR_ALIGN_8K = 13, /* 8K字节对齐 */
    MEM_ADDR_ALIGN_16K = 14, /* 16K字节对齐 */
    MEM_ADDR_BUTT /* 字节对齐非法 */
};

/*
 * @ingroup SRE_mem
 * 总体内存使用情况查询结构体。
 * 传递查询总体内存使用情况参数信息。
 */
struct MemPtUsage {
    U32 threadSize; /* 在任务或软中断中通过SRE_MemAlloc MID为0申请到内存大小，包含内存块控制头等。 */
    U32 hwiSize;    /* 所有硬中断在本内存分区中占用的内存总大小。 */
    U32 sysSize;    /* 包含OS控制资源、任务栈资源、调用OS接口申请的内存、初始化阶段调用。 */
};

struct MemCtrlHead {
    /* 魔术字，位置固定 */
    U16 magicWord;
    /* 分区号，位置固定 */
    U8 ptNo;
    /* 内存算法标志，位置固定 */
    U8 arithFlag;
    /* 该字段长度可根据需要扩展 */
    U16 reserved2;
    /* 控制头地址相对于用户地址的偏移，位置固定为控制头的末尾 */
    U16 headOffset;
};

typedef U32 (*MemPtCreateFunc)(U8 ptNo, struct MemPtCreate *para);
typedef U32 (*MemPtDeleteFunc)(U8 ptNo);
typedef void *(*MemAllocFunc)(U32 mid, U8 ptNo, uintptr_t size, uintptr_t alignPow);
typedef U32 (*MemFreeFunc)(U32 mid, U8 ptNo, struct MemCtrlHead *memHead);
typedef U32 (*MemPtInfoGetFunc)(U8 ptNo, struct MemStatInfo *memPtState);
#if defined(OS_OPTION_MEM_DBG)
typedef void (*MemDbgInfoGetFunc)(U8 ptNo, void *buff, uintptr_t *size);
typedef uintptr_t (*MemDbgInfoSizeGetFunc)(U8 ptNo);
#endif  // defined(OS_OPTION_MEM_DBG)
#if defined(OS_OPTION_MEM_DSSC)
typedef U32 (*MemFreeExFunc)(U32 mid, U8 ptNo, uintptr_t addr);
#endif

struct MemArithApi {
    MemPtCreateFunc ptCreateV2; /* 创建一个内存分区 */
#if defined(OS_OPTION_MEM_PT_DELETE)
    MemPtDeleteFunc ptDelete; /* 删除一个内存分区 */
#endif
    MemAllocFunc alloc; /* 申请一个内存块 */
    MemFreeFunc free; /* 释放一个内存块 */
    MemPtInfoGetFunc ptInfoGet; /* 获取分区基本信息 */
#if defined(OS_OPTION_MEM_DBG)
    MemDbgInfoGetFunc dbgInfoGet; /* 获取分区维测信息 */
    MemDbgInfoSizeGetFunc dbgInfoSizeGet; /* 获取分区维测信息大小 */
#endif
};

#if defined(OS_OPTION_MEM_USAGE_DETAIL_GET)
/*
 * @ingroup SRE_mem
 * 单个任务和软中断内存使用情况查询结构体。
 * 传递查询单个任务和软中断内存使用情况参数信息。
 */
struct MemTask {
    U16 id; /* 任务和软中断ID */
    U16 size; /* 在任务和软中断中通过SRE_MemAlloc和SRE_MemAllocAlign MID为0申请到内存大小，包含内存块控制头等 */
};
#endif

/*
 * @ingroup  SRE_mem
 * @brief 内存块被踩处理钩子函数类型定义。
 *
 * @par 描述
 * 当发生内存块被踩时，通过该钩子通知用户。
 * @attention
 * <ul>
 * <li>对于FSC算法，在内存释放时会进行内存头部踩踏检测（包括重复释放或被踩），
 * 将用户传入要释放的地址作为钩子函数参数。</li>
 * <li>对于POOL算法，在内存释放时会进行内存头部踩踏检测（仅限于被踩），将用户传入要释放的地址作为钩子函数参数。</li>
 * <li>对于POOL算法，在内存申请时会进行内存头部踩踏检测（仅限于被踩），将内存块起始地址作为钩子函数参数。</li>
 * </ul>
 * @param userAddr [IN]  类型#uintptr_t，内存块起始地址或用户传入要释放的地址。
 *
 * @retval 无。
 * @par 依赖
 * <ul><li>sre_mem.h：该接口声明所在的头文件。</li></ul>
 * @see 无。
 */
typedef void (*MemDamageHook)(uintptr_t userAddr);

/*
 * @ingroup SRE_mem
 * @brief 向LibCK已创建的指定分区申请内存。
 *
 * @par 描述
 * 在分区号为ptNo的分区中，申请大小为size的内存。
 * @attention
 * <ul>
 * <li>申请内存时的分区号，根据实际创建的分区号来使用。</li>
 * <li>调用函数后，注意判断返回的地址是否为空以避免后续访问空指针。</li>
 * <li>对于FSC或SLUB-FSC内存算法，申请到的内存首地址是按4字节对齐的，</li>
 * <li>对于POOL内存算法，申请到的内存首地址是按16字节对齐的。</li>
 * <li>如果内存申请失败，返回值为NULL，而导致失败的原因将记录在错误处理空间中。</li>
 * <li>DSW分区内存申请时，不关中断，只多核加锁，业务确保同一个核不会出现重复申请/释放导致死锁。</li>
 * </ul>
 *
 * @param mid  [IN]  类型#U32，申请的模块号。
 * @param ptNo [IN]  类型#U8，分区号，范围[0,#OS_MEM_MAX_PT_NUM+2)。
 * @param size [IN]  类型#uintptr_t，申请的大小。
 *
 * @retval #NULL  0，申请失败。
 * @retval #!NULL 内存首地址值。
 * @par 依赖
 * <ul><li>sre_mem.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_MemFree
 */
extern void *SRE_MemAlloc(U32 mid, U8 ptNo, uintptr_t size);

#if (defined OS_OPTION_MEM_DSW)

// OS_MEM_NOT_ENOUGH内存申请失败的错误类型宏值，业务会直接使用，涉及独立升级，不能改
#define OS_MEM_NOT_ENOUGH ((U32)-1)

/*
 * @ingroup SRE_mem
 * @brief 向LibCK已创建的指定DSW分区申请内存。
 *
 * @par 描述
 * 在分区号为ptNo的分区中，申请大小为size的内存。
 * @attention
 * <ul>
 * <li>申请内存时的分区号，根据实际创建的分区号来使用。</li>
 * <li>该接口只支持DSW算法分区，用于内存碎片整理。</li>
 * <li>DSW分区内存申请时，不关中断，只多核加锁，业务确保同一个核不会出现重复申请/释放导致死锁。</li>
 * </ul>
 *
 * @param mid      [IN]   类型#U32，申请的模块号。
 * @param ptNo     [IN]   类型#U8，分区号。
 * @param size     [IN]   类型#uintptr_t，申请的大小。
 * @param addr     [OUT]  类型#uintptr_t *，返回0(SRE_OK)时，携带申请到的地址。
 * @param freeSize [OUT]  类型#uintptr_t *，返回-1(OS_MEM_NOT_ENOUGH)时，携带分区剩余内存。
 *
 * @retval #OS_ERRNO_MEM_INPUT_INVAILD           0x0200010e，入参为空（addr or freeSize）。
 * @retval #OS_ERRNO_MEM_PTNO_INVALID            0x02000110，非DSW算法内存分区。
 * @retval #OS_ERRNO_MEM_ALLOC_SIZE_ZERO         0x02000111，申请size为0
 * @retval #OS_ERRNO_MEM_PT_NOT_CREATED          0x02000112，分区被删除了
 * @retval #OS_MEM_NOT_ENOUGH                    ((U32)-1)，分区内存申请不到。
 * @retval #SRE_OK                               0，内存成功。
 * @par 依赖
 * <ul><li>sre_mem.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_MemAlloc
 */
extern U32 SRE_MemAllocEx(U32 mid, U8 ptNo, uintptr_t size, uintptr_t *addr, uintptr_t *freeSize);
#endif

/*
 * @ingroup SRE_mem
 * @brief 向LibCK已创建的指定分区申请指定大小且指定对齐方式的内存块。
 *
 * @par 描述
 * 在分区号为ptNo的分区中，申请大小为size的内存，对齐方式为alignPow。
 * @attention
 * <ul>
 * <li>申请内存分区时的分区号，根据实际创建的分区号来使用。</li>
 * <li>调用函数后，注意判断返回的地址是否为空以避免后续访问空指针。</li>
 * <li>alignPow是对齐方式，对于POOL算法，最低只支持16字节对齐方式。</li>
 * <li>如果内存申请失败，返回值为NULL，而导致失败的原因将记录在错误处理空间中。</li>
 * </ul>
 *
 * @param mid      [IN]  类型#U32，申请的模块号。
 * @param ptNo     [IN]  类型#U8，分区号，范围[0,#OS_MEM_MAX_PT_NUM+2), MapPool算法分区号为[128, 256)。
 * @param size     [IN]  类型#uintptr_t，申请的大小。
 * @param alignPow [IN]  类型#enum MemAlign，动态对齐。
 *
 * @retval NULL  0，申请失败。
 * @retval !NULL，内存首地址值。
 * @par 依赖
 * <ul><li>sre_mem.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_MemFree
 */
extern void *SRE_MemAllocAlign(U32 mid, U8 ptNo, uintptr_t size, enum MemAlign alignPow);

/*
 * @ingroup SRE_mem
 * @brief 释放申请的内存。
 *
 * @par 描述
 * 该接口根据内存块的地址addr，找到该内存块所属的内存分区，再根据分区号和用户使用的地址addr释放该内存。
 * @attention
 * <ul>
 * <li>如果返回值不是#SRE_OK，则内存不会被释放。</li>
 * <li>被破坏的内存不能被释放。</li>
 * <li>对于入参addr，OS已做基本校验，无法校验所有非法地址，其合法性由业务保证。</li>
 * </ul>
 *
 * @param mid  [IN]  类型#U32，要释放的模块号。
 * @param addr [IN]  类型#void *，释放的地址。
 *
 * @retval #OS_ERRNO_MEM_FREE_PTNO_INVALID  0x02000104，释放内存所在的分区无效(分区未创建或分区号大于分区数)。
 * @retval #OS_ERRNO_MEM_FREE_SH_DAMAGED    0x0200011a，内存释放时要释放的内存块的头被破坏，或已释放。
 * @retval #OS_ERRNO_MEM_ATTR_INVALID       0x02000121，调用接口#SRE_MemFree/#SRE_MemCacheFree释放不可cache内存，
 * @retval                                              调用接口#SRE_MemUncacheFree释放可cache内存。
 * @retval #OS_ERRNO_MEM_FREE_ADDR_INVALID  0x02000103，释放地址为空。
 * @retval #SRE_OK                          0x00000000，内存释放成功。
 * @par 依赖
 * <ul><li>sre_mem.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_MemAlloc | SRE_MemAllocAlign
 */
extern U32 SRE_MemFree(U32 mid, void *addr);

#if defined(OS_OPTION_MEM_MAP_POOL) || defined(OS_OPTION_MEM_DSW) || defined(OS_OPTION_MEM_DSSC)
/*
 * @ingroup SRE_mem
 * @brief 释放申请的内存。
 *
 * @par 描述
 * 该接口通过指定内存分区，释放addr的内存。
 * @attention
 * <ul>
 * <li>MAP-POOL、DSSC和DSW分区内存只能通过该接口释放回内存池</li>
 * <li>如果返回值不是#SRE_OK，则内存不会被释放。</li>
 * <li>被破坏的内存不能被释放。</li>
 * <li>DSW分区内存释放时，不关中断，只多核加锁，业务确保同一个核不会出现重复申请/释放导致死锁。</li>
 * </ul>
 *
 * @param mid  [IN]  类型#U32，要释放的模块号。
 * @param ptNo [IN]  类型#U8，释放内存所属的内存分区。
 * @param addr [IN]  类型#const void *，释放的地址。
 *
 * @retval #OS_ERRNO_MEM_FREE_PTNO_INVALID       0x02000104，释放内存所在的分区无效(分区未创建或分区号大于分区数)。
 * @retval #OS_ERRNO_MEM_FREE_SH_DAMAGED         0x0200011a，内存释放时要释放的内存块的头被破坏，或已释放。
 * @retval #OS_ERRNO_MEM_ATTR_INVALID            0x02000121，调用接口#SRE_MemFree/#SRE_MemCacheFree释放不可cache内存，
 * @retval                                                   调用接口#SRE_MemUncacheFree释放可cache内存。
 * @retval #OS_ERRNO_MEM_FREE_ADDR_INVALID       0x02000103，释放地址为空。
 * @retval #OS_ERRNO_MEM_PT_NOT_CREATED          0x02000112，分区未创建
 * @retval #OS_ERRNO_DSSC_FREE_ADDR_ILLEGAL      0x02000153，内存地址非法，不是已申请的内存
 * @retval #OS_ERRNO_MEM_MAP_POOL_INVALID_ADDR   0x02000145，MapPool内存地址非法
 * @retval #OS_ERRNO_MEM_MAP_POOL_REPEAT_FREE    0x02000148，MapPool内存重复释放
 * @retval #OS_ERRNO_MEM_MAP_POOL_FATAL_ERROR    0x02000149，MapPool内存控制块被破坏
 * @retval #OS_ERRNO_DSW_MEM_FREE_REPEAT         0x02000155，DSW内存重复释放
 * @retval #OS_ERRNO_DSW_MEM_INVALID_ADDR        0x0200015b，DSW内存地址非法
 * @retval #SRE_OK                               0x00000000，内存释放成功。
 * @par 依赖
 * <ul><li>sre_mem.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_MemAlloc | SRE_MemAllocAlign
 */
extern U32 SRE_MemFreeEx(U32 mid, U8 ptNo, const void *addr);
#endif

/*
 * @ingroup SRE_mem
 * @brief 动态创建分区。
 *
 * @par 描述
 * 创建一个分区属性为para，分区号为ptNo的内存分区。
 * @attention
 * <ul>
 * <li>创建内存分区时的分区号，取值范围是[2,#OS_MEM_MAX_PT_NUM+2), MapPool算法分区号为[128, 256)。</li>
 * <li>用户可以在配置文件(sre_config.h)中配置系统支持的最大分区数#OS_MEM_MAX_PT_NUM，上限为253。</li>
 * <li>除了私有FSC算法，其它算法都可以通过内存裁剪开关决定是否创建。 </li>
 * <li>dsp系列平台，os在支持核间通信的前提下，会占用2个分区(不计入系统支持的最大分区数内)，</li>
 * <li>分区号从0到1，0号为系统使用的私有FSC分区、1号为系统使用的共享静态FSC分区；</li>
 * <li>否则，os占用一个分区号为0的私有FSC分区。</li>
 * <li>符号表模块打开的情况下，SD6108/81/82，Cortex-MX平台会占用一个内存分区。</li>
 * <li>创建内存时，用户配置的内存地址决定该内存是否可cache。</li>
 * </ul>
 *
 * @param para [IN]  类型#struct MemPtCreate *，待创建分区的信息。
 * @param ptNo [IN]  类型#U8，分区号。
 *
 * @retval #OS_ERRNO_MEM_PTCREATE_PARAMETER_INVALID 0x02000109，创建分区入参有误。
 * @retval #OS_ERRNO_MEM_PT_ALREADY_CREATED         0x0200010a，分区已经创建。
 * @retval #OS_ERRNO_MEM_PT_OVERLAP                 0x0200010b，动态创建内存分区时，区段有重叠。
 * @retval #OS_ERRNO_MEM_PTNO_INVALID               0x02000110，分区号非法。
 * @retval #OS_ERRNO_MEM_SHARE_PARA_INCONSISTENT    0x0200011c，共享分区参数配置不统一。
 * @retval                                                      不同核创建相同编号的共享分区时参数应相同。
 * @retval #OS_ERRNO_MEM_ALGORITHM_UNINSTALLED      0x0200010c，内存算法没有创建。
 * @retval #OS_ERRNO_MEM_PTCREATE_SIZE_TOOSMALL     0x02000101，创建分区的分区大小太小。
 * @retval #OS_ERRNO_MEM_PT_ARITH_INVALID           0x02000114, 分区算法不合法。
 * @retval #OS_ERRNO_FSCMEM_ALLOC_NO_MEMORY         0x0200012f，创建分区时，
 * @retval                                                      从缺省私有FSC内存分区中申请分区控制块失败。
 * @retval #OS_ERRNO_MCDYN_FSCMEM_ALLOC_NO_MEMORY   0x0200011d，创建分区时，
 * @retval                                                      从缺省共享静态FSC内存分区中申请分区控制块失败。
 * @retval #OS_ERRNO_MEM_PTCREATE_ADDR_TOOLARGE     0x02000126，分区起始地址太大。
 * @retval #OS_ERRNO_MEM_PTCREATE_ARITH_NONSUPPORT  0x02000127，多核宏OS_OPTION_AMP_MULTICORE未打开，
 * @retval                                                      不支持创建共享算法内存分区。
 * @retval #SRE_OK                                  0x00000000，分区创建成功。
 * @par 依赖
 * <ul><li>sre_mem.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_MemFree | SRE_MemAlloc
 */
extern U32 SRE_MemPtCreate(struct MemPtCreate *para, U8 ptNo);

#if defined(OS_OPTION_MEM_PT_DELETE)
/*
 * @ingroup SRE_mem
 * @brief 动态删除分区。
 *
 * @par 描述
 * 删除分区号为ptNo的内存分区。
 * @attention
 * <ul>
 * <li>仅支持删除私有内存算法(包括私有FSC算法和私有POOL算法)分区和共享SLUB-FSC算法和共享MapPool分区。</li>
 * <li>SMP系统由用户确保分区删除时该分区空闲，不会再从中申请。</li>
 * <li>不能删除系统缺省的分区。</li>
 * <li>不能删除未创建的分区。</li>
 * <li>调用接口会强制删除分区，分区对应的内存全部清零。</li>
 * <li>SMP系统由用户确保分区删除时该分区空闲，不会再从中申请。</li>
 * </ul>
 *
 * @param ptNo [IN]  类型#U8，待删除分区的分区号，取值范围是[2,#OS_MEM_MAX_PT_NUM + 2)，MapPool算法分区号为[128, 256)。
 *
 * @retval #OS_ERRNO_MEM_PTNO_INVALID                0x02000110，分区号非法。
 * @retval #OS_ERRNO_MEM_DELETE_DEFAULT_PT           0x02000124，删除系统缺省的分区。
 * @retval #OS_ERRNO_MEM_PT_NOT_CREATED              0x02000112，分区未创建。
 * @retval #OS_ERRNO_MEM_DELETE_SHARE_PT             0x02000125，删除共享内存算法分区。
 * @retval #OS_ERRNO_MEM_FREE_PTNO_INVALID           0x02000104，释放内存所在的分区无效(分区未创建或分区号大于分区数)。
 * @retval #OS_ERRNO_MEM_FREE_SH_DAMAGED             0x0200011a，内存释放时要释放的内存块的头被破坏，或已释放。
 * @retval #OS_ERRNO_MEM_FREE_ADDR_INVALID           0x02000103，释放地址为空。
 * @retval #SRE_OK                                   0x00000000，分区删除成功。
 * @par 依赖
 * <ul><li>sre_mem.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_MemPtCreate
 */
extern U32 SRE_MemPtDelete(U8 ptNo);
#endif

/*
 * @ingroup SRE_mem
 * @brief 获取指定内存分区的内存占用率。
 *
 * @par 描述
 * 获取分区号为ptNo的内存分区的占用率。
 * @attention
 * <ul>
 * <li>对于POOL算法，已使用的内存空间包括申请之后释放的内存空间，由于内存申请之后，即使释放，其内存块类型已经</li>
 * <li>被指定，不能再自由分配。</li>
 * <li>对于FSC和SLUB-FSC算法，已使用的内存空间不包括申请之后释放的内存空间，释放的空间可以重新被分配。</li>
 * <li>分区号不存在时该分区的内存占用率为0。</li>
 * </ul>
 *
 * @param ptNo [IN]  类型#U32，分区号，范围[0,#OS_MEM_MAX_PT_NUM+2)，
 *                   但实例间共享分区为[#OS_MI_SHARE_MEM_PT_FLAG,#OS_MI_SHARE_MEM_MAX_PT_NUM+16)。
 *
 * @retval 内存分区占用率   操作成功，分区的内存占用率取值范围为[0,10000]。
 * @retval 0                该分区的内存占用率为0或者该分区不存在。
 * @par 依赖
 * <ul><li>sre_mem.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_MemPtCreate
 */
extern U32 SRE_MemUsageGet(U32 ptNo);

/*
 * @ingroup SRE_mem
 * @brief 查询内存号为ptNo的内存分区的基本信息，将信息返回给memPtState所指向的结构体。
 *
 * @par 描述
 * 输入分区号，查询该分区的总大小，历史使用峰值和剩余内存的大小。
 * @attention
 * <ul>
 * <li>支持POOL算法和FSC算法、SLUB-FSC算法。</li>
 * <li>内存分区总大小是指用户申请的分区大小与该分区控制块的大小的总和。</li>
 * <li>历史使用峰值是指分区历史使用最大的内存空间的大小。</li>
 * <li>对于POOL算法来说，由于内存申请之后，即使释放，其内存块类型已经被指定，不能再自由分配。</li>
 * <li>所以对于POOL算法，即使内存申请之后释放，这块空间也算作被使用的；</li>
 * <li>对于FSC或SLUB-FSC算法，申请了又被释放的内存可以重新分配，算作未被使用。</li>
 * </ul>
 *
 * @param ptNo       [IN]  类型#U32，分区号，范围[0,#OS_MEM_MAX_PT_NUM+2)，
 *                         但实例间共享分区为[#OS_MI_SHARE_MEM_PT_FLAG,#OS_MI_SHARE_MEM_MAX_PT_NUM+16)。
 * @param memPtState [OUT] 类型#struct MemStatInfo *，保存内存分区的使用状态信息。
 *
 * @retval #OS_ERRNO_MEM_INPUT_INVAILD   0x0200010e，结构体为空。
 * @retval #OS_ERRNO_MEM_NO_USE          0x0200010d，分区未创建或分区号非法。
 * @retval #OS_ERRNO_MEM_ARITH_INVALID   0x0200011b，内存算法类型非法或OS不支持该内存算法。
 * @retval #SRE_OK                       0x00000000，操作成功。
 * @par 依赖
 * <ul><li>sre_mem.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_MemPtCreate
 */
extern U32 SRE_MemPtStateGet(U32 ptNo, struct MemStatInfo *memPtState);

#if defined(OS_OPTION_MEM_UNCACHE_ALLOC)
/*
 * @ingroup SRE_mem
 * @brief 向LibCK已创建的分区OS_MEM_DEFAULT_FSC_PT申请指定大小的默认可cache内存块。
 *
 * @par 描述
 * 在分区号为OS_MEM_DEFAULT_FSC_PT的分区中，申请大小为size的内存，对齐方式为alignPow。
 * @attention
 * <ul>
 * <li>此接口只适用于Cortex_AX平台Balong产品。</li>
 * <li>如果内存申请失败，返回值为NULL，而导致失败的原因将记录在错误处理空间中。</li>
 * </ul>
 *
 * @param size     [IN]  类型#uintptr_t，申请的大小。
 * @param alignPow [IN]  类型#enum MemAlign，动态对齐，有效取值范围为#OS_MEM_ALIGN_E。
 *
 * @retval #NULL  0，申请失败。
 * @retval #!NULL 内存首地址值。
 * @par 依赖
 * <ul><li>sre_mem.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_MemCacheFree
 */
OS_SEC_ALW_INLINE INLINE void *SRE_MemCacheAlloc(uintptr_t size, enum MemAlign alignPow)
{
    return SRE_MemAllocAlign(0, OS_MEM_DEFAULT_FSC_PT, size, alignPow);
}

/*
 * @ingroup SRE_mem
 * @brief 释放申请的内存。
 *
 * @par 描述
 * 该接口根据内存块的地址addr，找到该内存块所属的内存分区，再根据分区号和用户使用的地址addr释放该内存。
 * @attention
 * <ul>
 * <li>如果返回值不是#SRE_OK，则内存不会被释放。</li>
 * <li>被破坏的内存不能被释放。</li>
 * <li>此接口只适用于Cortex_AX平台Balong产品。</li>
 * </ul>
 *
 * @param addr [IN]  类型#void *，释放的地址。
 *
 * @retval #OS_ERRNO_MEM_FREE_PTNO_INVALID         0x02000104，释放内存所在的分区无效(分区未创建或分区号大于分区数)。
 * @retval #OS_ERRNO_MEM_FREE_SH_DAMAGED           0x0200011a，内存释放时要释放的内存块的头被破坏，或已释放。
 * @retval #OS_ERRNO_MEM_FREE_POOL_MEM_ALREADY_FREE
 * 0x0200013e，私有POOL内存释放时内存块已经被释放(或者内存没有被申请过)。
 * @retval #OS_ERRNO_MEM_FREE_SHARE_MEM_ALREADY_FREE
 * 0x02000143，共享动态POOL内存释放时内存块已经被释放(或者内存没有被申请过)。
 * @retval #OS_ERRNO_MEM_ATTR_INVALID
 * 0x02000121，调用接口#SRE_MemFree/#SRE_MemCacheFree释放不可cache内存，调用接口#SRE_MemUncacheFree释放可cache内存。
 * @retval #OS_ERRNO_MEM_FREE_ADDR_INVALID         0x02000103，释放地址为空。
 * @retval #SRE_OK                                 0x00000000，内存释放成功。
 * @par 依赖
 * <ul><li>sre_mem.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_MemCacheAlloc
 */
OS_SEC_ALW_INLINE INLINE U32 SRE_MemCacheFree(void *addr)
{
    return SRE_MemFree(0, (void *)addr);
}

/*
 * @ingroup SRE_mem
 * @brief 向LibCK已创建的分区OS_MEM_DEFAULT_FSC_PT申请指定大小的默认不可cache内存块。
 *
 * @par 描述
 * 在分区号为OS_MEM_DEFAULT_FSC_PT的分区中，申请大小为size的内存，对齐方式为alignPow。
 * @attention
 * <ul>
 * <li>此接口只适用于Cortex_AX平台Balong产品。</li>
 * <li>内存对齐方式，至少满足4k字节对齐。</li>
 * <li>如果内存申请失败，返回值为NULL，而导致失败的原因将记录在错误处理空间中。</li>
 * </ul>
 *
 * @param size     [IN]  类型#uintptr_t，申请的大小。
 * @param alignPow [IN]  类型#enum MemAlign，动态对齐，有效取值范围为#OS_MEM_ALIGN_E。
 *
 * @retval #NULL  0，申请失败。
 * @retval #!NULL 内存首地址值。
 * @par 依赖
 * <ul><li>sre_mem.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_MemUncacheFree
 */
extern void *SRE_MemUncacheAlloc(uintptr_t size, enum MemAlign alignPow);

/*
 * @ingroup SRE_mem
 * @brief 向默认分区释放指定的地址不可cache内存。
 *
 * @par 描述
 * 该接口根据内存块的地址addr，找到该内存块所属的内存分区，再根据分区号和用户使用的地址addr释放该内存。
 * @attention
 * <ul>
 * <li>如果返回值不是#SRE_OK，则内存不会被释放。</li>
 * <li>被破坏的内存不能被释放。</li>
 * <li>此接口只适用于Cortex_AX平台Balong产品。</li>
 * </ul>
 *
 * @param addr [IN]  类型#void *，释放的地址。
 *
 * @retval #OS_ERRNO_MEM_FREE_PTNO_INVALID           0x02000104，释放内存所在的分区无效(分区未创建或分区号大于分区数)。
 * @retval #OS_ERRNO_MEM_FREE_INVALID                0x02000122，调用接口#SRE_MemUncacheFree释放
 * @retval                                                       非OS_MEM_DEFAULT_FSC_PT分区中的内存。
 * @retval #OS_ERRNO_MEM_FREE_SH_DAMAGED             0x0200011a，内存释放时要释放的内存块的头被破坏，或已释放。
 * @retval #OS_ERRNO_MEM_FREE_POOL_MEM_ALREADY_FREE  0x0200013e，私有POOL内存释放时内存块已经被释放
 * @retval                                                        (或者内存没有被申请过)。
 * @retval #OS_ERRNO_MEM_FREE_SHARE_MEM_ALREADY_FREE 0x02000143，共享动态POOL内存释放时内存块已经被释放
 * @retval                                                       (或者内存没有被申请过)。
 * @retval #OS_ERRNO_MEM_ATTR_INVALID                0x02000121，调用接口#SRE_MemFree/#SRE_MemCacheFree释放不可cache
 * @retval                                                       内存，调用接口#SRE_MemUncacheFree释放可cache内存。
 * @retval #OS_ERRNO_MEM_FREE_ADDR_INVALID           0x02000103，释放地址为空。
 * @retval #OS_ERRNO_MEM_MMU_NOT_INIT                0x02000123，mmu模块没有初始化。
 * @retval #OS_ERRNO_MMU_DEOPTIMIZE_FAIL             0x02001211，页表拆分时内存不足。
 * @retval #OS_ERRNO_MMU_ADDR_NOT_MAPPED             0x02001210，传入虚拟地址未映射过。
 * @retval #SRE_OK                                   0x00000000，内存释放成功。
 * @par 依赖
 * <ul><li>sre_mem.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_MemUncacheAlloc
 */
extern U32 SRE_MemUncacheFree(void *addr);
#endif

/*
 * @ingroup SRE_mem
 * @brief 注册踩内存处理钩子函数。
 *
 * @par 描述
 * 根据用户配置的回调函数hook，注册踩内存处理钩子函数。
 * @attention
 * <ul>
 * <li>不允许重复或覆盖注册钩子。hook为NULL时，表示删除该钩子。</li>
 * <li>支持在内存块申请、释放过程对内存块头部踩踏进行检测。</li>
 * <li>对于fsc算法，不区分内存块重复释放和内存块头部踩踏，两种场景都会执行回调函数；</li>
 * <li>对于pool算法，区分内存块重复释放和内存块头部踩踏两种场景，内存块头部踩踏会执行回调函数，</li>
 * <li>而内存块重复释放仅返回错误码。</li>
 * </ul>
 *
 * @param hook [IN]  类型#MemDamageHook，内存踩踏回调函数。
 *
 * @retval #OS_ERRNO_HOOK_TYPE_INVALID              0x02001600，HOOK类型错误。
 * @retval #OS_ERRNO_HOOK_FULL                      0x02001604，HOOK已满或配置个数为0。
 * @retval #SRE_OK                                  0x00000000，钩子注册成功。
 * @par 依赖
 * <ul><li>sre_mem.h：该接口声明所在的头文件。</li></ul>
 * @see 无。
 */
extern U32 SRE_MemDamageHookReg(MemDamageHook hook);

#if defined(OS_OPTION_MEM_APPEND)
/*
 * @ingroup SRE_mem
 * @brief 挂载内存块到默认分区
 *
 * @par 描述
 * 挂载内存块到默认分区
 * @attention
 * <ul>
 * <li>只支持挂载1个内存块到默认分区</li>
 * <li>挂载的内存块会作为1个内部分区，采用fsc算法进行内存内存管理。</li>
 * </ul>
 *
 * @param addr   [IN] 类型#void *，要挂接到默认内存分区的内存块起始地址。
 * @param size   [IN] size  类型#unsigned int，要挂接到默认内存分区的内存块大小。
 *
 * @retval #OS_ERRNO_MEM_APPEND_PARA_INVALID        0x02000137，入参有误。
 * @retval #OS_ERRNO_MEM_PTCREATE_SIZE_TOOSMALL     0x02000101，挂载内存块大小太小。
 * @retval #SRE_OK                                  0x00000000，挂接成功。
 * @par 依赖
 * <ul><li>sre_mem.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_MemFree | SRE_MemAlloc
 */
extern U32 SRE_MemDefPtAppend(void *addr, uintptr_t size);

/*
 * @ingroup SRE_mem
 * @brief 挂载内存块到指定分区
 *
 * @par 描述
 * 挂载内存块到指定分区
 * @attention
 * <ul>
 * <li>支持挂载多个内存块到指定分区。</li>
 * <li>只支持fsc、dssc内存算法。</li>
 * </ul>
 *
 * @param ptNo   [IN] 类型#U8，要挂载内存块的分区号。
 * @param memCfg [IN] 类型#struct MemAppendCfg *，内存块的信息结构体指针。
 * @param num    [IN] 类型#U32，要挂接到指定内存分区的内存块个数。
 *
 * @retval #OS_ERRNO_MEM_PTNO_INVALID               0x02000110，分区号不合法。
 * @retval #OS_ERRNO_MEM_PT_NOT_CREATED             0x02000112，分区未创建。
 * @retval #OS_ERRNO_MEM_APPEND_PARA_INVALID        0x02000137，入参有误。
 * @retval #OS_ERRNO_MEM_PTCREATE_SIZE_TOOSMALL     0x02000101，挂载内存块大小太小。
 * @retval #OS_ERRNO_MEM_APPEND_OVERLAP             0x02000135，内存分区之间有重叠。
 * @retval #OS_ERRNO_DSSC_ALLOC_HEAD_NO_MEMORY      0x02000152，创建分区时，从缺省私有FSC内存分区中申请分区控制块失败。
 * @retval #SRE_OK                                  0x00000000，挂接成功。
 * @par 依赖
 * <ul><li>sre_mem.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_MemFree | SRE_MemAlloc
 */
extern U32 SRE_MemPtAppend(U8 ptNo, struct MemAppendCfg *memCfg, U32 num);

/*
 * @ingroup SRE_mem
 * @brief 删除指定分区挂载的内存块
 *
 * @par 描述
 * 删除指定分区挂载的内存块
 * @attention
 * <ul>
 * <li>只支持fsc内存算法。</li>
 * </ul>
 *
 * @param ptNo   [IN] 类型#U8，要删除挂载内存块的分区号。
 * @param memCfg [IN] 类型#struct MemAppendCfg *，内存块的信息结构体指针。
 * @param num    [IN] 类型#U32，要删除的挂载内存块个数。
 *
 * @retval #OS_ERRNO_MEM_PTNO_INVALID               0x02000110，分区号不合法。
 * @retval #OS_ERRNO_MEM_PT_NOT_CREATED             0x02000112，分区未创建。
 * @retval #OS_ERRNO_MEM_APPEND_PARA_INVALID        0x02000137，入参有误。
 * @retval #OS_ERRNO_MEM_APPEND_REMOVE_ADDR_INVALID 0x02000138，内存地址不合法。
 * @retval #OS_ERRNO_MEM_APPEND_REMOVE_ERROR        0x02000139，挂载内存都已经释放。
 * @retval #SRE_OK                                  0x00000000，删除成功。
 * @par 依赖
 * <ul><li>sre_mem.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_MemFree | SRE_MemAlloc
 */
extern U32 SRE_MemPtRemove(U8 ptNo, struct MemAppendCfg *memCfg, U32 num);
#endif

#if defined(OS_OPTION_MEM_DBG)
/*
 * @ingroup SRE_mem
 * @brief 向LibCK已创建的指定分区申请内存, 并记录用户信息。
 *
 * @par 描述
 * 在分区号为ptNo的分区中，申请大小为size的内存。
 * @attention
 * <ul>
 * <li>仅支持FSC和SLUB-FSC算法</li>
 * <li>申请内存时的分区号，根据实际创建的分区号来使用。</li>
 * <li>调用函数后，注意判断返回的地址是否为空以避免后续访问空指针。</li>
 * <li>对于FSC和SLUB-FSC内存算法，申请到的内存首地址是按4字节对齐的。</li>
 * <li>如果内存申请失败，返回值为NULL，而导致失败的原因将记录在错误处理空间中。</li>
 * </ul>
 *
 * @param mid    [IN]  类型#U32，申请的模块号。
 * @param ptNo   [IN]  类型#U8，分区号，范围[0,#OS_MEM_MAX_PT_NUM+2)。
 * @param size   [IN]  类型#uintptr_t，申请的大小。
 * @param cookie [IN]  类型#U32，用户标记。
 *
 * @retval #NULL  0，申请失败。
 * @retval #!NULL 内存首地址值。
 * @par 依赖
 * <ul><li>sre_mem.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_MemFree
 */
extern void *SRE_MemAllocDbg(U32 mid, U8 ptNo, uintptr_t size, U32 cookie);

/*
 * @ingroup SRE_mem
 * @brief 向LibCK已创建的指定分区申请指定大小且指定对齐方式的内存块,并记录维测信息。
 *
 * @par 描述
 * 在分区号为ptNo的分区中，申请大小为size的内存，对齐方式为alignPow。
 * @attention
 * <ul>
 * <li>仅支持FSC和SLUB-FSC算法。</li>
 * <li>申请内存分区时的分区号，根据实际创建的分区号来使用。</li>
 * <li>调用函数后，注意判断返回的地址是否为空以避免后续访问空指针。</li>
 * <li>alignPow是对齐方式，最低4字节对齐。</li>
 * <li>如果内存申请失败，返回值为NULL，而导致失败的原因将记录在错误处理空间中。</li>
 * </ul>
 *
 * @param mid      [IN]  类型#U32，申请的模块号。
 * @param ptNo     [IN]  类型#U8，分区号，范围[0,#OS_MEM_MAX_PT_NUM+2)。
 * @param size     [IN]  类型#uintptr_t，申请的大小。
 * @param alignPow [IN]  类型#enum MemAlign，动态对齐。
 * @param cookie   [IN]  类型#U32，用户标记。
 *
 * @retval NULL  0，申请失败。
 * @retval !NULL，内存首地址值。
 * @par 依赖
 * <ul><li>sre_mem.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_MemFree
 */
extern void *SRE_MemAllocAlignDbg(U32 mid, U8 ptNo, uintptr_t size, enum MemAlign alignPow, U32 cookie);

/*
 * @ingroup SRE_mem
 * @brief 向LibCK已创建的分区OS_MEM_DEFAULT_FSC_PT申请指定大小的默认可cache内存块并记录cookie。
 *
 * @par 描述
 * 在分区号为OS_MEM_DEFAULT_FSC_PT的分区中，申请大小为size的内存，对齐方式为alignPow，并记录cookie。
 * @attention
 * <ul>
 * <li>此接口只适用于Cortex_RX平台Balong产品。</li>
 * <li>如果内存申请失败，返回值为NULL，而导致失败的原因将记录在错误处理空间中。</li>
 * </ul>
 *
 * @param size     [IN]  类型#uintptr_t，申请的大小。
 * @param alignPow [IN]  类型#enum MemAlign，动态对齐，有效取值范围为#OS_MEM_ALIGN_E。
 * @param cookie   [IN]  类型#U32，用户传入cookie供OS记录和后续解析。
 *
 * @retval #NULL  0，申请失败。
 * @retval #!NULL 内存首地址值。
 * @par 依赖
 * <ul><li>sre_mem.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_MemCacheFree
 */
OS_SEC_ALW_INLINE INLINE void *SRE_MemCacheAllocDbg(uintptr_t size, enum MemAlign alignPow, U32 cookie)
{
    return SRE_MemAllocAlignDbg(0, OS_MEM_DEFAULT_FSC_PT, size, alignPow, cookie);
}

#if defined(OS_OPTION_MEM_UNCACHE_ALLOC)
/*
 * @ingroup SRE_mem
 * @brief 向LibCK已创建的分区OS_MEM_DEFAULT_FSC_PT申请指定大小的默认不可cache内存块，并记录cookie。
 *
 * @par 描述
 * 在分区号为OS_MEM_DEFAULT_FSC_PT的分区中，申请大小为size的内存，对齐方式为alignPow。
 * @attention
 * <ul>
 * <li>此接口只适用于Cortex_RX平台Balong产品。</li>
 * <li>如果内存申请失败，返回值为NULL，而导致失败的原因将记录在错误处理空间中。</li>
 * </ul>
 *
 * @param size     [IN]  类型#uintptr_t，申请的大小。
 * @param alignPow [IN]  类型#enum MemAlign，动态对齐，有效取值范围为#OS_MEM_ALIGN_E。
 * @param cookie   [IN]  类型#U32，用户传入cookie供OS记录和后续解析。
 *
 * @retval #NULL  0，申请失败。
 * @retval #!NULL 内存首地址值。
 * @par 依赖
 * <ul><li>sre_mem.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_MemUncacheFree
 */
extern void *SRE_MemUncacheAllocDbg(uintptr_t size, enum MemAlign alignPow, U32 cookie);
#endif
/*
 * @ingroup SRE_mem
 * @brief 获取指定分区的内存维测信息大小，大小单位为字节
 *
 * @par 描述
 * 获取内存分区维测信息总大小。
 * @attention
 * <ul>
 * <li>无</li>
 * </ul>
 *
 * @param ptNo [IN]  类型#U8，分区号，范围[0,#OS_MEM_MAX_PT_NUM+2)。
 *
 * @retval 维测信息大小。
 * @retval 0， 维测信息获取失败。
 * @par 依赖
 * <ul><li>sre_mem.h：该接口声明所在的头文件。</li></ul>
 * @see 无。
 */
extern U32 SRE_MemDbgInfoSizeGet(U8 ptNo);

/*
 * @ingroup SRE_mem
 * @brief 获取内存维测信息。
 *
 * @par 描述
 * 获取内存分区内未释放内存块的维测信息。
 * @attention
 * <ul>
 * <li>每个内存块的维测信息按OS_MEM_DBG_INFO_S存储，占12字节。</li>
 * <li>如果BUFF不能够保存所有内存块维测信息，则根据BUFF大小能保存多少就保存多少。</li>
 * </ul>
 *
 * @param ptNo [IN]  类型#U8，分区号，范围[0,#OS_MEM_MAX_PT_NUM+2)。
 * @param buff [OUT] 类型#void *，用于保存维测信息的BUFF。
 * @param size [I/O] 类型#uintptr_t *，输入输出参数，输入: 用于保存维测信息的BUFF的大小，输出: 保存的维测信息实际大小。
 *
 * @retval #OS_ERRNO_MEM_DBG_INFO_PARA_NULL         0x02000128，指针类型参数为NULL。
 * @retval #OS_ERRNO_MEM_PTNO_INVALID               0x02000110，分区号非法。
 * @retval #OS_ERRNO_MEM_PT_NOT_CREATED             0x02000112，分区号未创建。
 * @retval #OS_ERRNO_MEM_DBG_GET_INFO_NULL          0x0200012e，该分区不支持维测信息获取。
 * @retval #SRE_OK                                  0x00000000，成功。
 * @par 依赖
 * <ul><li>sre_mem.h：该接口声明所在的头文件。</li></ul>
 * @see 无。
 */
extern U32 SRE_MemDbgInfoGet(U8 ptNo, void *buff, uintptr_t *size);
#endif

#if defined(OS_OPTION_MEM_USAGE_DETAIL_GET)
/*
 * @ingroup SRE_mem
 * @brief 查询内存使用情况，将信息返回给memPtUsage所指向的结构体。
 *
 * @par 描述
 * 输入分区号，查询任务、软中断、硬中断申请的内存大小。
 * @attention
 * <ul>
 * <li>仅支持私有FSC算法。</li>
 * <li>仅支持Cortex_M4平台。</li>
 * <li>在TICK中的用户内存消耗统计到HWI中。</li>
 * <li>在系统初始化后，对输入模块号0的内存申请进行线程级统计，非0模块的申请直接计入系统消耗中。</li>
 * <li>任务切换钩子中的消耗计入系统消耗中。</li>
 * </ul>
 *
 * @param ptNo       [IN]  类型#U8，分区号，范围[0,#OS_MEM_MAX_PT_NUM+2)。
 * @param memPtUsage [OUT] 类型#struct MemPtUsage *，内存分区使用情况汇总,保存内存分区的使用状态信息。
 * @param maxNum     [I/O] 类型#U16 *，输入缓冲区数组的个数，输出填写在缓冲区memTask中有效的元素个数。
 * @param memThread  [OUT] 类型#struct MemTask *，缓冲区，用于填写任务和软中断的内存占用信息。
 *
 * @retval #OS_ERRNO_MEMINFO_CHECK_ALL_ADDR_NULL     0x02000115，输入的有效参数都为NULL。
 * @retval #OS_ERRNO_MEMINFO_TSK_SINGLE_ADDR_NULL    0x02000116，查询单个内存使用情况时，
 * @retval                                                       输入的两个有效参数其中一个为NULL。
 * @retval #OS_ERRNO_MEMINFO_CHECK_TASKNUM_INVALID   0x02000117，查询内存使用情况时，输入的查询任务个数等于0。
 * @retval #OS_ERRNO_MEM_PTNO_INVALID                0x02000110, 分区号非法。
 * @retval #OS_ERRNO_MEM_PT_NOT_CREATED              0x02000112, 分区未创建。
 * @retval #OS_ERRNO_MEM_PT_ARITH_NOT_SUPPORT        0x02000119, 分区算法不支持。
 * @retval #SRE_OK                                   0x00000000，操作成功。
 * @par 依赖
 * <ul><li>sre_mem.h：该接口声明所在的头文件。</li></ul>
 * @see SRE_MemPtCreate
 */
extern U32 SRE_MemPtUsage(U8 ptNo, struct MemPtUsage *memPtUsage, U16 *maxNum, struct MemTask *memThread);
#endif

#if (defined OS_OPTION_MEM_DSW)
/*
 * @ingroup SRE_mem
 * @brief 配置DSW算法所需的状态字内存。
 *
 * @par 描述
 * 该接口配置DSW算法的状态字内存，整个实例共用。
 * @attention
 * <ul>
 * <li>配置内存地址和大小必须4字节对齐</li>
 * </ul>
 *
 * @param swAddr [IN]  类型#uintptr_t，状态字内存首地址。
 * @param swSize [IN]  类型#U32，状态字内存大小。
 *
 * @retval #OS_ERRNO_DSW_SW_CFG_INVALID          0x02000155，无效状态字内存配置
 * @retval #SRE_OK                               0x00000000，状态字内存注册成功。
 * @par 依赖
 * <ul><li>sre_mem.h：该接口声明所在的头文件。</li></ul>
 * @see
 */
extern U32 SRE_DswMemSwCfg(uintptr_t swAddr, U32 swSize);
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* SRE_MEM_H */
