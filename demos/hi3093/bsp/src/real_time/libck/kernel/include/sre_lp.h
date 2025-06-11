/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2009-2019. All rights reserved.
 * Description: 多核LP模块的头文件。
 * Author: LibCK项目组
 * Create: 2009-12-22
 * History: 2018-10-27 LibCK项目组  华为规范整改
 */
#ifndef SRE_LP_H
#define SRE_LP_H

#include "sre_base.h"
#include "sre_sys.h"
#include "./common/os_lp.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*
 * *@defgroup sre_lp 动态加载和补丁模块
 * @ingroup sre_lp
 *
 * *
 * @ingroup sre_lp
 * 基础bin组名定义
 */
#define OS_LP_BASE_BIN_GROUP_NAME 0x0

/*
 * @ingroup sre_lp
 *
 * 日期的结构定义。
 */
struct DateStru {
    /* 年，范围1970~2100 */
    U16 year;
    /* 月，范围1~12 */
    U8 month;
    /* 日，范围1~31 */
    U8 date;
};

/*
 * @ingroup sre_lp
 *
 * 时间的结构定义。
 */
struct TimeStru {
    /* 时，范围0~23 */
    U8 hour;
    /* 分，范围0~59 */
    U8 minute;
    /* 秒，范围0~59 */
    U8 second;
    /* 保留 */
    U8 padding;
};

/*
 * @ingroup sre_lp
 * LP单元状态枚举定义。
 * UNIT_STATE_ACTIVATING   : 正在激活状态。
 * UNIT_STATE_ACTIVATED    : 已经激活状态。
 * UNIT_STATE_DEACTIVATING : 正在去激活状态。
 * UNIT_STATE_DEACTIVATED  : 已经去激活状态。
 * UNIT_STATE_IDLE         : 空闲状态。
 * UNIT_STATE_BUTT         : 非法状态。
 */
enum SreLPState {
    UNIT_STATE_ACTIVATED = 0x01, /* 激活状态。 */
    UNIT_STATE_DEACTIVATED = 0x02, /* 去激活状态。 */
    UNIT_STATE_IDLE = 0x03, /* 空闲状态。 */
    UNIT_STATE_ACTIVATING = 0x04, /* 激活过程态。 */
    UNIT_STATE_DEACTIVATING = 0x05, /* 去激活过程态。 */
    UNIT_STATE_BUTT /* 非法状态。 */
};

/*
 * @ingroup sre_lp
 * @brief LP状态存储函数类型定义。
 *
 * @par 描述
 * LP单元状态存储函数钩子类型定义，在LP加载、去除、运行和去运行时调用该钩子进行LP单元状态的存储。
 * @attention
 * <ul>
 * <li>仅SD6182 /SD6157支持</li>
 * </ul>
 *
 * @param imageId   [IN]  类型#U32，静态配置的镜像。
 * @param groupName [IN]  类型#U32，注册组信息时的组名。
 * @param unitNo    [IN]  类型#U32，工具端制作LP单元包时，用户指定的单元号。
 * @param unitState [IN]  类型#enum SreLPState，工具端制作LP单元包时，用户指定的单元号。
 *
 * @retval #SRE_OK 0，成功返回。
 * @retval #OS_ERROR -1，成功失败。
 *
 * @par 依赖
 * sre_lp.h：该函数类型声明所在的头文件
 * @see 无。
 */
typedef U32(*SreSavePatchStateFuncPtr)(U32 imageId, U32 groupName, U32 unitNo, enum SreLPState unitState);

/*
 * @ingroup sre_lp
 * LP组注册段重叠错误信息上报结构体定义。
 */
struct LPErrReportInfo {
    /* 比较段所属组镜像ID */
    U32 compImageId;
    /* 比较段所属组名 */
    U32 compGroupName;
    /* 比较段名，如果静态补丁段名为SP_CODE,SP_SHARED,SP_PRIVATE */
    const char *compLpName;
    /* 比较的段索引 */
    U32 comIndex;
    /* 比较段起始地址 */
    uintptr_t compLpAddr;
    /* 比较段长度 */
    uintptr_t compLpSize;
    /* 重叠段所属组镜像ID */
    U32 overlayImageId;
    /* 重叠段所属组名 */
    U32 overlayGroupName;
    /* 重叠段名，如果静态补丁段名为SP_CODE,SP_SHARED,SP_PRIVATE */
    const char *overlayLpName;
    /* 重叠段的段索引 */
    U32 overLayIndex;
    /* 重叠段起始地址 */
    uintptr_t overLayLpAddr;
    /* 重叠段长度 */
    uintptr_t overLaySize;
};

/*
 * @ingroup sre_lp
 * @brief 配置段重叠错误信息存储函数类型定义。
 *
 * @par 描述
 * 配置段重叠错误信息存储函数类型定义，方便注册时对段重叠错误的定位。
 * @attention
 * <ul>
 * <li>仅SD6182/SD6157支持</li>
 * </ul>
 *
 * @param errorReportInfo [IN]  类型#struct LPErrReportInfo *，LP组注册段重叠错误信息上报结构体定义。
 *
 * @retval void。
 *
 * @par 依赖
 * sre_lp.h：该函数类型声明所在的头文件
 * @see 无。
 */
typedef void (*LPErrReportHook)(struct LPErrReportInfo *errorReportInfo);

/*
 * @ingroup sre_lp
 * @brief 多核SP补丁区信息
 */
struct HPAreaInfo {
    /* 补丁区是否在共享内存,08/81只有L1是私有内存，82只有Local L2是私有内存 */
    bool areaIsShare;
    /* 补丁代码区起始地址 */
    uintptr_t codeAreaAddr;
    /* 补丁代码区大小 */
    uintptr_t codeAreaSize;
    /* 补丁共享数据区起始地址 */
    uintptr_t dataShareAreaAddr;
    /* 补丁共享数据区大小 */
    uintptr_t dataShareAreaSize;
    /* 补丁私有数据区起始地址 */
    uintptr_t dataPriAreaAddr;
    /* 补丁数据区大小 */
    uintptr_t dataAreaSize;
};

/*
 * @ingroup sre_lp
 * @brief 多核LP动态补丁或动态加载段区信息
 */
struct SreDLDPAreaInfo {
    /* 用户配置段的名字，段名字不能重复 */
    const char *secName;
    /*
     * 段起始地址，cacheLine对齐（取CPU cacheLine和DSP cacheLine的最大值），代码段的起始地址必须配置在0x40000000以下
     */
    uintptr_t secAreaAddr;
    /* 段大小，单位：byte，要求cacheLine的整数倍 */
    uintptr_t secAreaSize;
    /* 是否组间共享数据段 */
    bool secShared;
};

/*
 * @ingroup sre_lp
 * 单元段信息定义
 */
struct LPGroupQueryInfo {
    /* 段的总大小 */
    U32 totalSize;
    /* 段空闲空间的总大小 */
    U32 freeSize;
    /* 段的最大可用大小 */
    U32 maxAvailableSize;
};

/*
 * @ingroup sre_lp
 * Load&Patch注册类型枚举定义。
 */
enum LPRegType {
    LP_TYPE_NULL = 0,  /* 非法LP类型 */
    LP_TYPE_SP = 1,    /* 静态补丁类型, 当前预留不支持 */
    LP_TYPE_DL = 2,    /* 动态加载类型 */
    LP_TYPE_DP = 4,    /* 动态补丁类型 */
    LP_TYPE_DP_DL = 6, /* 动态补丁和动态加载类型 */
};

/*
 * @ingroup sre_lp
 * @brief 依赖组信息结构体定义
 */
struct LPDependGroupInfo {
    /* 依赖组名字数组 */
    U32 *dependGrpNameBuf;
    /* 依赖组个数 */
    U32 dependGrpNum;
};

/*
 * @ingroup sre_lp
 * @brief 依赖单元信息结构体定义
 */
struct UnitDependedInfo {
    /* 注册时返回的组ID */
    U32 groupId;
    /* 工具端制作LP单元包时，用户指定的单元号 */
    U32 unitNo;
};

/*
 * @ingroup sre_lp
 * LP组注册信息结构体定义。
 */
struct LPGroupRegInfo {
    /* 组支持的类型                  (作用域:动态库(DL)+动态补丁(DP)、动态库(DL)、动态补丁(DP)) */
    enum LPRegType type;
    /*
     * 组的生效核范围，BitMap表,每个bit对应一个物理核，
     * 生效核不能跨基础bin(仅Xtensa有效)(作用域:动态库+动态补丁、动态库、动态补丁)
     */
    U32 coreBitTab;
    /* 单元最大函数替换个数          (作用域:动态补丁) */
    U32 patMaxFuncNum;
    /* 多核同步超时时间，单位ms      (仅Xtensa有效)(作用域:动态加载、动态补丁) */
    U32 overTime;
    /* 镜像支持的核的个数，          (仅Xtensa有效)(作用域:静态补丁) ，当前预留不支持 */
    U32 dssNum;
    /*
     * 段个数（动态库/动态补丁）
     * (仅Xtensa有效)(作用域:动态库+动态补丁、动态库、动态补丁)
     */
    U32 areaNum;
    /* 静态补丁的补丁区信息          (仅Xtensa有效)(作用域:静态补丁) ，当前预留不支持 */
    struct HPAreaInfo *areaInfo;
    /* 动态库动态补丁的段信息        (仅Xtensa有效)(作用域:动态库+动态补丁、动态库、动态补丁) */
    struct SreDLDPAreaInfo *dldpSecInfo;
    /* 搜索组个数                    (仅Xtensa有效)(作用域:动态库+动态补丁、动态库、动态补丁) */
    U32 searchNum;
    /* 搜索链，数组形式，每个元素对应一个组名 (仅Xtensa有效)(作用域:动态库+动态补丁、动态库、动态补丁) */
    U32 *searchLink;
};

/*
 * @ingroup sre_lp
 *
 * 动态库段信息获取的入参结构定义。
 */
struct SreDLibSpec {
    /* 动态库名 */
    U8 *dlibName;
    /* 段名 */
    U8 *secName;
};

/*
 * @ingroup sre_lp
 *
 * 动态库段信息获取的出参结构定义。
 */
struct DLibSecInfo {
    /* 段起始虚地址 */
    uintptr_t addr;
    /* 段长度 */
    uintptr_t len;
};

/*
 * @ingroup sre_lp
 * Load&Patch调测Trace枚举。
 */
enum LPLoadTrace {
    LP_FETCH_ACTIVE_ENTRY = 0, /* OsLPUnitFetchActive,接收到主核消息，开始动态加载 */
    LP_MCPU_FETCH_ENTRY = 1, /* OsLPMcpuUnitFetchActive,开始主核动态加载 */
    LP_DL_FETCH_ENTRY = 2, /* OsLPDLFetch,开始动态库加载 */
    LP_DP_FETCH_ENTRY = 3, /* OsLPDPFetch,开始动态补丁加载 */
    LP_DYN_ELF_RELOCA = 4, /* OsLPElfFetchRelocate,进入ELF解析及重定位 */
    LP_ALL_CORE_CACHE_INV = 5, /* OsLPAllCoreCacheInv,开始所有核无效CACHE */
    LP_DYN_TBL_TBL_BUILD = 6, /* OsLPDynSymTabBuild,开始添加动态符号到符号表 */
    LP_ALL_CORE_SYN_FETCH = 7, /* OsLPAllCoreSynFetch,所有核开始同步加载重定位 */
    LP_RELOCA_AND_RELOAD = 8, /* OsLPRelocAndReload,进入重定位和段加载流程 */
    LP_MMU_SEC_PROC = 9, /* OsLPMmuSecProc,开始段拷贝 */
    LP_INST_RELOCAL = 10, /* OsLPSecRelocBuild,开始指令的重定位 */
    LP_SEC_RELOAD = 11, /* OsLPReLoad,进行段的cache无效 */
    LP_INST_RELOC_AGAIN = 12, /* OsLPUnRelocAgain,对非基础bin的符号进行再重定位，双向依赖场景 */
    LP_ACTIVE_ENTRY = 13, /* OsLPActive,开始动态库激活 */
    LP_DL_EXC_FUN_ENTRY = 14, /* OsLPExcFunc,开始动态库出入口函数执行 */
    LP_DP_REPLACE_ENTRY = 15, /* OsLPReplacePatch,开始动态库补丁函数替换 */
    LP_DL_EXC_FUN_PROC = 16, /* OsLPExcAMPPatch,同步执行出入口函数并返回消息 */
    LP_DP_ROLL_BACK_FUN = 17, /* OsLPRollbackFun,回退补丁函数 */
    LP_DP_REPLACE_PATCH_FUN = 18, /* OsLPReplacePatchFun,替换补丁函数 */
    LP_EXC_USER_FUN_OVER = 19, /* OsLPExcPatchFun,动态库出入口函数执行结束 */
    LP_ALL_CORE_FETCH_OVER = 20, /* OsLPUnitFetchActive,动态库加载激活完成开始发送消息给主核 */
    LP_MCPU_FETCH_OVER = 21, /* OsLPMcpuUnitFetchActive,主核动态加载完成 */
    LP_SECONDARY_REMOVE_ENTRY = 22, /* OsLPSecondaryRemove,接收到主核消息，开始动态库卸载 */
    LP_REMOVE_ENTRY = 23, /* OsLPRemove,开始动态库卸载 */
    LP_UNIT_ROLLBACK = 24, /* OsLPRollback,开始回退动态库或者动态补丁 */
    LP_UNIT_DEACTIVE_START = 25, /* OsLPDeactiveProc,进入动态去激活流程 */
    LP_UNIT_DEACTIVE_END = 26, /* OsLPDeactiveProc,去激活流程完成 */
    LP_ALL_CORE_SYN_REMOVE = 27, /* OsLPAllCoreSynRemove,进行所有核的动态单元同步移除 */
    LP_UNIT_CTRL_FREE = 28, /* OsLPUnitSecRollBack,进行单元控制块释放 */
    LP_SECONDARY_REMOVE_OVER = 29, /* OsLPSecondaryRemove,动态库卸载完成开始发送消息给主核 */
    LP_MCPU_REMOVE_OVER = 30, /* OsLPMCPURemove,主核动态库卸载完成 */
};

#if (defined OS_OPTION_LP_PRELINK)
/*
 * @ingroup sre_lp
 * Load&Patch动态库入口参数定义。
 */
enum EntryType {
    CORE_ACTIVED = 0, // 已激活核参数
    CORE_ACTIVE = 1,  // 待激活核参数
};

/*
 * @ingroup sre_lp
 * Load&Patch动态库出口参数定义。
 */
enum ExitType {
    CORE_NOT_DEACTIVE = 0, // 非去激活核参数
    CORE_DEACTIVE = 1, // 待去激活核参数
};

/*
 * @ingroup sre_lp
 * @brief APP Start 钩子函数原型定义。
 *
 * @par 描述
 * 平台注册APP Start钩子函数，在实例激活时调用。
 * @attention
 * <ul>
 * <li>仅SD6185X支持</li>
 * </ul>
 *
 * @param type [IN]  类型#enum EntryType，标明核是否已激活。
 *
 * @retval U32。
 *
 * @par 依赖
 * sre_lp.h：该函数类型声明所在的头文件
 * @see SRE_LPAppStartHookAdd。。
 */
typedef U32 (*AppStartHook)(enum EntryType type);

/*
 * @ingroup sre_lp
 * @brief APP Stop 钩子函数原型定义。
 *
 * @par 描述
 * 平台注册APP Stop钩子函数，在实例去激活时调用。
 * @attention
 * <ul>
 * <li>仅SD6185X支持</li>
 * </ul>
 *
 * @param type [IN]  类型#enum ExitType，标明核是否要去激活。
 *
 * @retval U32。
 *
 * @par 依赖
 * sre_lp.h：该函数类型声明所在的头文件
 * @see SRE_LPAppStopHookAdd
 */
typedef U32 (*AppStopHook)(enum ExitType type);

/*
 * @ingroup sre_lp
 * @brief 注册APP Start钩子
 *
 * @par 描述
 * 平台调用该函数注册钩子，最多注册32个。
 *
 * @attention
 * <ul>
 * <li>仅SD6185X支持</li>
 * </ul>
 * @param hook    [IN]  类型#AppStartHook，hook钩子函数。
 *
 * @retval #OS_ERRNO_LP_APP_START_HOOK_NULLPTR           0x02001eb1,钩子函数为空。
 * @retval #OS_ERRNO_LP_APP_START_HOOK_FULL              0x02001eb2,钩子函数注册满。
 * @retval #SRE_OK                                       0x00000000,成功注册钩子。
 *
 * @par 依赖
 * <ul><li>sre_lp.h：该接口声明所在的头文件。</li></ul>
 * @see
 */
extern U32 SRE_LPAppStartHookAdd(AppStartHook hook);

/*
 * @ingroup sre_lp
 * @brief 注册APP Stop钩子
 *
 * @par 描述
 * 平台调用该函数注册钩子，最多注册32个。
 *
 * @attention
 * <ul>
 * <li>仅SD6185X支持</li>
 * </ul>
 * @param hook    [IN]  类型#AppStartHook，hook钩子函数。
 *
 * @retval #OS_ERRNO_LP_APP_STOP_HOOK_NULLPTR           0x02001eb3,钩子函数为空。
 * @retval #OS_ERRNO_LP_APP_STOP_HOOK_FULL              0x02001eb4,钩子函数注册满。
 * @retval #SRE_OK                                      0x00000000,成功注册钩子。
 *
 * @par 依赖
 * <ul><li>sre_lp.h：该接口声明所在的头文件。</li></ul>
 * @see
 */
extern U32 SRE_LPAppStopHookAdd(AppStopHook hook);
#endif

/*
 * @ingroup sre_lp
 * @brief 获取动态库的段信息
 *
 * @par 描述
 * 根据动态库的库名以及段名获取段信息。
 *
 * @attention
 * <ul>
 * <li>仅SD6183及以上平台支持</li>
 * </ul>
 * @param dlibSpec    [IN]  类型#struct SreDLibSpec *，入参信息，包括动态库名和段名。
 * @param dlibSecInfo [OUT] 类型#struct DLibSecInfo *，出参信息，输出段的虚地址和长度。
 *
 * @retval #OS_ERRNO_LP_INPUT_IS_NULL                    0x02001e0d,参数指针为空。
 * @retval #OS_ERRNO_LP_GET_INST_INFO_FIND_FAILED        0x02001eaa,实例信息有名空间内未找到。
 * @retval #OS_ERRNO_LP_DLIB_NAME_FIND_FAILED            0x02001eab,未找到对应的动态库名。
 * @retval #OS_ERRNO_LP_DLIB_SEC_FIND_FAILED             0x02001eac,未找到对应的段名。
 * @retval #SRE_OK                                       0x00000000,成功获取段信息。
 *
 * @par 依赖
 * <ul><li>sre_lp.h：该接口声明所在的头文件。</li></ul>
 * @see
 */
extern U32 SRE_GetDlibSecInfo(struct SreDLibSpec *dlibSpec, struct DLibSecInfo *dlibSecInfo);

/*
 * @ingroup sre_lp
 * @brief LP管理组信息注册函数。
 *
 * @par 描述
 * 注册LP组信息函数。
 * @attention
 * <ul>
 * <li>仅SD6182/SD6157支持</li>
 * <li>动态加载和动态补丁areaNum的值和dldpSecInfo 中配置的节区个数要一致。</li>
 * <li>动态加载和动态补丁配置的节区名字不能为空和重复，节区之间也不能重合。</li>
 * <li>SD6182/SD6157补丁区的代码段或代码节区配置的地址空间不能超过0x40000000地址。否则使用会出问题。</li>
 * <li>接口中镜像ID的正确性需要用户保证，必须是OS_SYS_IMAGE_ID。</li>
 * <li>如果使用动态加载，动态补丁，需要打开符号表裁剪开关。</li>
 * <li>动态加载及动态补丁组信息注册时地址不能重叠，而且需要128字节对齐(考虑cache line)，段名字不能重复。</li>
 * <li>注册接口通过imageId注册，多次注册如果参数一致返回都SRE_OK，并输出同一个动态加载及动态补丁管理信息组ID。</li>
 * <li>调用注册接口注册成功n次，需要调用清除注册接口n次，最后才真正删除注册信息。</li>
 * </ul>
 *
 * @param imageId   [IN]  类型#U32，镜像ID，一个镜像对应一个基础BIN，镜像id (SD6182平台[0,16])，需要用户保证其正确性。
 * @param groupName [IN]  类型#U32，管理信息组名，镜像内唯一，组名0x0预留给基础BIN使用。
 * @param groupInfo [IN]  类型#struct LPGroupRegInfo *，组信息注册指针，组信息，包含段配置、搜索链、生效核等。
 * @param groupId   [OUT] 类型#U32 *，组ID，由注册接口注册后分配，后续的LP加载、激活等操作都将需使用组ID。
 *
 * @retval #OS_ERRNO_LP_GROUP_ID_INVALID                           0x02001e72，LP管理组ID不合法。
 * @retval #OS_ERRNO_LP_INFO_REPEAT_DIFFER                         0x02001e17，重复注册的LP组信息不一致。
 * @retval #OS_ERRNO_LP_MANAGE_UNIT_NOT_USED                       0x02001e18，LP管理信息组未被初始化。
 * @retval #OS_ERRNO_LP_IMAGE_ID_INVALID                           0x02001e2a，镜像ID错误。
 * @retval #OS_ERRNO_LP_INPUT_IS_NULL                              0x02001e0d，入参为空。
 * @retval #OS_ERRNO_LP_BUF_NOT_ENOUGH                             0x02001e0e，LP缓冲区长度不正确。
 * @retval #OS_ERRNO_LP_IDENT_INVALID                              0x02001e20，不是合法的LP文件。
 * @retval #OS_ERRNO_LP_FILE_IS_INVALID                            0x02001e16，LP文件长度不正确。
 * @retval #OS_ERRNO_LP_PROTOCOL_VER_INVALID                       0x02001e05，LP文件结构变更升级版本号错误。
 * @retval #OS_ERRNO_LP_SEC_NAME_SAME                              0x02001e19，动态加载，动态补丁段名配置相同。
 * @retval #OS_ERRNO_LP_DLDP_AREA_OVERLAY_IN_SAME_GROUP            0x02001e84，同一个Group内动态补丁或
 * @retval                                                                     动态加载段之间有配置重叠。
 * @retval #OS_ERRNO_LP_DLDP_AREA_OVERLAY_IN_ANOTHER_GROUP         0x02001e7f，不同Group间动态补丁或
 * @retval                                                                     动态加载段之间有配置重叠。
 * @retval #OS_ERRNO_LP_DLDP_AREA_OVERLAY_IN_ANOTHER_GROUP_WITH_SP 0x02001e73，不同Group间动态补丁或动态加载段与
 * @retval                                                                     已配置的静态补丁的代码段，共享数据段或
 * @retval                                                                     私有数据段有配置重叠。
 * @retval #OS_ERRNO_LP_NOT_INITED                                 0x02001e48，LP模块未初始化，没有经过镜像初始化。
 * @retval #OS_ERRNO_LP_DLDPAREAINFO_IS_NULL                       0x02001e4a，动态补丁动态加载必须配置段区。
 * @retval #OS_ERRNO_LP_CFG_SEC_INVALID                            0x02001e4b，动态加载动态补丁的段区的起始地址或者
 * @retval                                                                     长度配置错误。
 * @retval #OS_ERRNO_LP_CFG_SEC_NAME_INVALID                       0x02001e4c，动态加载动态补丁的段区的名字不能为空。
 * @retval #OS_ERRNO_LP_GLB_L2_ADDR                                0x02001e53，LOCAL L2地址不允许配置为全局地址。
 * @retval #OS_ERRNO_LP_DEPEND_SYMBOL_INITED                       0x02001e55，动态加载，动态补丁需要依赖符号表模块。
 * @retval #OS_ERRNO_LP_REG_NOT_FIND_IDLE_GROUP                    0x02001e71，LP注册时找不到补丁空闲的管理组。
 * @retval #OS_ERRNO_LP_REG_FUNC_NUM_INVALID                       0x02001e24，LP单元最大函数个数配置为0。
 * @retval #OS_ERRNO_LP_CORE_BIT_MAP_INVALID                       0x02001e75，LP管理组生效核范围错误。
 * @retval #OS_ERRNO_LP_CODE_PUT_IN_ANOTHER_1G                     0x02001e2f，LP的代码区超过了0x40000000。
 * @retval #OS_ERRNO_LP_REG_NOT_ALIGN                              0x02001e23，配置的补丁区或段区(起始地址及长度)
 * @retval                                                                     未按照cacheline对齐(SD6182为128字节)，
 * @retval                                                                     或者私有补丁区未按照4字节对齐。
 * @retval #OS_ERRNO_LP_SHARE_SECTION_ADDR_IN_LL2                  0x02001e86，共享段地址不可配置为LOCAL L2内存。
 * @retval #OS_ERRNO_LP_DLDP_SHARE_AREA_OVERLAY_IN_ANOTHER_GROUP   0x02001e88，两个组的共享段配置有重叠或者
 * @retval                                                                     共享段属性不一致。
 * @retval #OS_ERRNO_LP_VALID_CORE_BIT_TAB_ERROR                   0x02001e89，用户配置的生效核范围配置错误。
 * @retval #OS_ERRNO_LP_RESET_NOT_GROUP_REG                        0x02001e92，LP单核复位还未完成时，
 * @retval                                                                     其他组进行组的注册操作。
 * @retval #OS_ERRNO_LP_RESET_CORE_ERR                             0x02001e94，LP单核复位操作时，
 * @retval                                                                     用来做复位操作的核不是故障核。
 * @retval #OS_ERRNO_LP_RESET_NOT_REG_NEW_GROUP                    0x02001e9c，LP单核复位时候不能创建之前未存在的组。
 * @retval #SRE_OK                                                 0x00000000，成功返回。
 *
 * @par 依赖
 * sre_lp.h：该接口声明所在的头文件
 * @see SRE_LPGroupInfoUnReg
 */
extern U32 SRE_LPGroupInfoReg(U32 imageId, U32 groupName, struct LPGroupRegInfo *groupInfo, U32 *groupId);

/*
 * @ingroup sre_lp
 * @brief LP管理组信息去注册函数。
 *
 * @par 描述
 * 去注册指定LP管理组ID对应的LP组信息函数。
 * @attention
 * <ul>
 * <li>仅SD6182/SD6157支持</li>
 * <li>去注册接口中会检测该LP管理组ID管理的所有LP单元的状态，如果所有LP的都是空闲，才可以清除注册该LP管理组信息。</li>
 * </ul>
 *
 * @param groupId [IN]  类型#U32，注册时返回的组ID，需要用户保证其正确性。
 *
 * @retval #OS_ERRNO_LP_GROUP_ID_INVALID                    0x02001e72，LP管理组ID不合法。
 * @retval #OS_ERRNO_LP_GROUP_ID_IN_USE                     0x02001e59，LP管理组ID在使用中，不能去注册该补丁信息管理组。
 * @retval #OS_ERRNO_LP_MANAGE_UNIT_NOT_USED                0x02001e18，LP管理信息组未被初始化。
 * @retval #OS_ERRNO_LP_RESET_NOT_GROUP_UNREG               0x02001e93，LP单核复位还未完成时，有组在做去注册操作。
 * @retval #SRE_OK                                          0x00000000，成功返回。
 *
 * @par 依赖
 * sre_lp.h：该接口声明所在的头文件
 * @see SRE_LPGroupInfoReg
 */
extern U32 SRE_LPGroupInfoUnReg(U32 groupId);

/*
 * @ingroup sre_lp
 * @brief 保存LP单元状态的钩子函数注册。
 *
 * @par 描述
 * 将保存LP单元状态的钩子注册到当前镜像中。
 * @attention
 * <ul>
 * <li>仅SD6182/SD6157支持</li>
 * <li>每个镜像注册一个LP单元状态保存钩子，用于记录LP操作过程中的LP单元状态。</li>
 * </ul>
 *
 * @param hook [IN]  类型#SreSavePatchStateFuncPtr，保存补丁状态的钩子函数类型。
 *
 * @retval #OS_ERRNO_LP_REG_STATESAVEHOOK_NULL            0x02001e29，注册保存补丁状态的钩子函数为空。
 * @retval #OS_ERRNO_LP_NOT_INITED                        0x02001e48，补丁模块未初始化，没有经过镜像初始化。
 * @retval #SRE_OK                                        0x00000000，成功返回。
 *
 * @par 依赖
 * sre_lp.h：该接口声明所在的头文件
 * @see SRE_LPGroupInfoReg
 */
extern U32 SRE_LPUnitStateSaveHookReg(SreSavePatchStateFuncPtr hook);

/*
 * @ingroup sre_lp
 * @brief 动态加载或补丁加载函数。
 *
 * @par 描述
 * 将缓冲区中的LP文件加载到指定LP管理组ID对应的补丁区或者指定的段中。
 * @attention
 * <ul>
 * <li>仅SD6182/SD6157支持</li>
 * <li>1、被打补丁函数大小不小于16字节。C语言编译出来的函数已经做了校验，有错误码返回，</li>
 * <li>elf文件不提供汇编语言编译出来的函数大小信息，故无法判断，需用户自行保证，否则系统会出现异常；</li>
 * <li>2、如果补丁单元之间出现符号重名或者有符号引用就会按照加载顺序产生依赖关系；</li>
 * <li>3、支持C递归补丁替换函数的CALL重定位,不支持汇编递归补丁替换函数的CALL重定位,因为汇编函数的符号大小为0。</li>
 * <li>4、ARM平台下补丁文件中使用的全局变量必须赋值或者指定段或者增加-fno-common编译选项，</li>
 * <li>否则补丁加载失败，无法找到符号。</li>
 * </ul>
 *
 * @param groupId [IN]  类型#U32，注册时返回的组ID，需要用户保证其正确性。
 * @param buf     [IN]  类型#U8 *，LP单元的加载包的起始地址，该起始地址必须4字节对齐，单元号包含在加载包中。
 * @param length  [IN]  类型#U32，LP单元的加载包的大小。
 *
 * @retval #OS_ERRNO_LP_BUF_ADDR_NOT_ALIGN                  0x02001e22, 缓冲区地址(buf)非4字节对齐。
 * @retval #OS_ERRNO_LP_GROUP_ID_INVALID                    0x02001e72，LP管理组ID不合法。
 * @retval #OS_ERRNO_LP_MANAGE_UNIT_NOT_USED                0x02001e18，指定LP管理信息组未被初始化。
 * @retval #OS_ERRNO_LP_INPUT_IS_NULL                       0x02001e0d，入参为空。
 * @retval #OS_ERRNO_LP_BUF_NOT_ENOUGH                      0x02001e0e，动态加载或补丁缓冲区长度不正确。
 * @retval #OS_ERRNO_LP_IDENT_INVALID                       0x02001e20，不是合法的LP文件。
 * @retval #OS_ERRNO_LP_FILE_IS_INVALID                     0x02001e16，动态加载或LP文件长度不正确。
 * @retval #OS_ERRNO_LP_PROTOCOL_VER_INVALID                0x02001e05，LP文件结构变更升级版本号错误。
 * @retval #OS_ERRNO_LP_LOADED                              0x02001e06，动态加载或补丁文件重复加载。
 * @retval #OS_ERRNO_LP_CALL_OFFECT_ERROR                   0x02001e1b，动态加载动态补丁调用
 * @retval                                                              非补丁替换函数CALL重定位错误。
 * @retval #OS_ERRNO_LP_REG_FUNC_NUM_INVALID                0x02001e24，LP单元最大函数个数配置为0。
 * @retval #OS_ERRNO_LP_SEC_ALIGN_ERROR                     0x02001e26，ELF段对齐错误。
 * @retval #OS_ERRNO_LP_ITEM_SIZE_ERROR                     0x02001e28，符号表段头中每个符号节点的大小错误。
 * @retval #OS_ERRNO_LP_INST_TYPE_ERROR                     0x02001e46，重定位符号对应的指令类型不支持。
 * @retval #OS_ERRNO_LP_RECUR_CALL_ERROR                    0x02001e63，动态补丁递归调用补丁替换函数call重定位失败。
 * @retval #OS_ERRNO_LP_NONRECUR_CALL_ERROR                 0x02001e65，动态补丁非递归补丁替换函数call重定位失败。
 * @retval #OS_ERRNO_LP_ELF_FILETYPE_ERROR                  0x02001e64，LP文件类型错误。
 * @retval #OS_ERRNO_LP_SEC_INFO_GET_ERROR                  0x02001e41，动态加载或动态补丁加载时获取段信息错误。
 * @retval #OS_ERRNO_LP_FETCH_TYPE_ERROR                    0x02001e43，LP加载时，LP包的类型和核支持的类型不符。
 * @retval #OS_ERRNO_LP_FETCH_DP_GOURP_ERROR                0x02001e7a，打补丁的LP组不为符号所在的组。
 * @retval #OS_ERRNO_LP_CODE_ADDR_NOT_SUIT                  0x02001e07，静态补丁文件中代码段与配置中的代码段不一致。
 * @retval #OS_ERRNO_LP_DATA_ADDR_NOT_SUIT                  0x02001e07，静态补丁文件中数据段与配置中的共享数据段或
 * @retval                                                              私有数据段不一致。
 * @retval #OS_ERRNO_LP_CODE_PUT_IN_ANOTHER_1G              0x02001e2f，LP的代码区超过了0x40000000。
 * @retval #OS_ERRNO_LP_SECTAB_MEM_ERROR                    0x02001e4d，节区表内存申请错误。
 * @retval #OS_ERRNO_LP_SYM_BUILD_ERROR                     0x02001e52，临时符号表生成错误。
 * @retval #OS_ERRNO_LP_FUNC_OVERFLOW                       0x02001e15，静态补丁或者动态补丁加载的补丁函数个数超过了
 * @retval                                                              最大的限制。
 * @retval #OS_ERRNO_LP_DATA_OVER_FLOW                      0x02001e13，静态补丁单元数据段开始地址小于补丁区数据段首地址
 * @retval                                                              或者静态补丁单元数据段超出补丁区数据段范围。
 * @retval #OS_ERRNO_LP_DP_DEPEND_DP_NOT_SAME_GROUP         0x02001e85，DP不能引用其他组DP的符号。
 * @retval #OOS_ERRNO_LP_DL_DEPEND_DP                       0x02001e87，DL不能引用DP的符号，不能依赖DP。
 * @retval #OS_ERRNO_LP_RESET_LOAD_FAIL                     0x02001e8c，LP单核复位时加载了之前未加载过的动态库或者
 * @retval                                                              动态补丁。
 * @retval #OS_ERRNO_LP_RESET_NOT_FETCH                     0x02001e8e，LP单核复位还未完成时，非故障核做动态补丁，
 * @retval                                                              动态库加载操作。
 * @retval #OS_ERRNO_LP_RESET_CORE_ERR                      0x02001e94，LP单核复位操作时，用来做复位操作的核不是故障核。
 * @retval #SRE_OK                                          0x00000000，成功返回。
 *
 * @par 依赖
 * sre_lp.h：该接口声明所在的头文件
 * @see SRE_LPUnitRemove
 */
extern U32 SRE_LPUnitFetch(U32 groupId, U8 *buf, U32 length);

/*
 * @ingroup sre_lp
 * @brief 动态加载包或补丁移除函数。
 *
 * @par 描述
 * 删除指定LP管理组ID对应的指定补丁或动态加载单元，同时删除依赖于该LP单元号的所有补丁或动态加载单元。
 * @attention
 * <ul>
 * <li>仅SD6182/SD6157支持</li>
 * <li>因为删除的(补丁或动态加载)单元没有备份，所以删除失败时已被删除的依赖的单元不会被回退。</li>
 * </ul>
 *
 * @param groupId [IN]  类型#U32，注册时返回的组ID，需要用户保证其正确性。
 * @param unitNo  [IN]  类型#U32，工具端制作LP单元包时，用户指定的单元号。
 *
 * @retval #OS_ERRNO_LP_GROUP_ID_INVALID                   0x02001e72，LP管理组ID不合法。
 * @retval #OS_ERRNO_LP_MANAGE_UNIT_NOT_USED               0x02001e18，LP管理信息组未被初始化。
 * @retval #OS_ERRNO_LP_NOT_LOADED                         0x02001e09，动态加载或补丁单元未加载。
 * @retval #OS_ERRNO_LP_MEM_NOT_ENOUGH                     0x02001e0f，内存申请失败，LP操作过程中需要动态申请内存。
 * @retval #OS_ERRNO_LP_RESET_NOT_REMOVE                   0x02001e91，LP单核复位还未完成时，有组在做动态补丁，
 * @retval                                                             动态库移除操作。
 * @retval #SRE_OK                                         0x00000000，成功返回。
 *
 * @par 依赖
 * sre_lp.h：该接口声明所在的头文件
 * @see SRE_LPUnitFetch
 */
extern U32 SRE_LPUnitRemove(U32 groupId, U32 unitNo);

/*
 * @ingroup sre_lp
 * @brief 动态加载或补丁激活函数。
 *
 * @par 描述
 * 激活指定LP管理组ID的指定LP单元，同时激活此LP单元号依赖的所有LP单元。
 * @attention
 * <ul>
 * <li>仅SD6182/SD6157支持</li>
 * <li>对于补丁来说激活操作实现函数替换，对于动态加载激活则执行动态加载的入口函数 。</li>
 * </ul>
 *
 * @param groupId [IN]  类型#U32，注册时返回的组ID，需要用户保证其正确性。
 * @param unitNo  [IN]  类型#U32，工具端制作LP单元包时，用户指定的单元号，其取值范围[1,9999]。
 *
 * @retval #OS_ERRNO_LP_GROUP_ID_INVALID                   0x02001e72，LP管理组ID不合法。
 * @retval #OS_ERRNO_LP_NO_INVALID                         0x02001e01，动态加载或补丁单元号错误。
 * @retval #OS_ERRNO_LP_MANAGE_UNIT_NOT_USED               0x02001e18，LP管理信息组未被初始化。
 * @retval #OS_ERRNO_LP_NOT_LOADED                         0x02001e09，动态加载或补丁单元未加载。
 * @retval #OS_ERRNO_LP_INPUT_IS_NULL                      0x02001e0d，入参为空。
 * @retval #OS_ERRNO_LP_MEM_NOT_ENOUGH                     0x02001e0f，内存申请失败，LP操作过程中需要动态申请内存。
 * @retval #OS_ERRNO_LP_IS_ACTIVE                          0x02001e0a，动态加载单元或补丁单元已经处于激活状态。
 * @retval #OS_ERRNO_LP_IS_RUNNING                         0x02001e0b，动态加载单元或补丁单元已经处于运行状态。
 * @retval #OS_ERRNO_LP_ALLOC_COMM_MEM_FAIL                0x02001e1d，同步过程中申请的共享内存失败。
 * @retval #OS_ERRNO_LP_SET_PARA_FAIL                      0x02001e1e，设置同步参数失败。
 * @retval #OS_ERRNO_LP_ACTIVE_FAIL                        0x02001e8a，LP单元激活失败。
 * @retval #OS_ERRNO_LP_RESET_NOT_ACTIVE                   0x02001e8f，LP单核复位还未完成时，非故障核做动态补丁，
 * @retval                                                             动态库激活操作。
 * @retval #OS_ERRNO_LP_RESET_CORE_ERR                     0x02001e94，LP单核复位操作时，用来做复位操作的核不是故障核。
 * @retval #OS_ERRNO_LP_RESET_ACTIVE_NOT_LOAD_UNIT         0x02001e9d，LP单核复位时候不能激活之前依赖却未加载的组。
 * @retval #SRE_OK                                         0x00000000，成功返回。
 *
 * @par 依赖
 * sre_lp.h：该接口声明所在的头文件
 * @see SRE_LPUnitDeactive
 */
extern U32 SRE_LPUnitActive(U32 groupId, U32 unitNo);

/*
 * @ingroup sre_lp
 * @brief 动态加载或补丁去激活函数。
 *
 * @par 描述
 * 去激活指定LP管理组ID的指定LP单元，同时去激活依赖于该LP单元号的所有LP单元。
 * @attention
 * <ul>
 * <li>仅SD6182/SD6157支持</li>
 * <li>对于补丁去激活操作是进行函数替换，对于动态加载去激活则执行动态加载的退出函数。</li>
 * </ul>
 *
 * @param groupId [IN]  类型#U32，注册时返回的组ID，需要用户保证其正确性。
 * @param unitNo  [IN]  类型#U32，工具端制作LP单元包时，用户指定的单元号，其取值范围[1, 9999]。
 *
 * @retval #OS_ERRNO_LP_GROUP_ID_INVALID                   0x02001e72，LP管理组ID不合法。
 * @retval #OS_ERRNO_LP_NO_INVALID                         0x02001e01，动态加载或补丁单元号错误。
 * @retval #OS_ERRNO_LP_MANAGE_UNIT_NOT_USED               0x02001e18，LP管理信息组未被初始化。
 * @retval #OS_ERRNO_LP_NOT_LOADED                         0x02001e09，动态加载或补丁单元未加载。
 * @retval #OS_ERRNO_LP_MEM_NOT_ENOUGH                     0x02001e0f，内存申请失败，LP操作过程中需要动态申请内存。
 * @retval #OS_ERRNO_LP_IS_DEACTIVE                        0x02001e0c，动态加载或补丁单元已经处于去激活状态。
 * @retval #OS_ERRNO_LP_IS_RUNNING                         0x02001e0b，动态加载或补丁单元已经处于运行状态。
 * @retval #OS_ERRNO_LP_INPUT_IS_NULL                      0x02001e0d，入参指针为空。
 * @retval #OS_ERRNO_LP_ALLOC_COMM_MEM_FAIL                0x02001e1d，同步过程中申请的共享内存失败。
 * @retval #OS_ERRNO_LP_SET_PARA_FAIL                      0x02001e1e，设置同步参数失败。
 * @retval #OS_ERRNO_LP_START_SYN_FAIL                     0x02001e1a，同步启动失败。
 * @retval #OS_ERRNO_LP_DEACTIVE_SYN_PHASE_FAIL            0x02001e1c，同步阶段操作失败。
 * @retval #OS_ERRNO_LP_DEACTIVE_FAIL                      0x02001e8b，LP单元去激活失败。
 * @retval #OS_ERRNO_LP_RESET_NOT_DEACTIVE                 0x02001e90，LP单核复位还未完成时，有组在做动态补丁，
 * @retval                                                             动态库去激活操作。
 * @retval #SRE_OK                                         0x00000000，成功返回。
 *
 * @par 依赖
 * sre_lp.h：该接口声明所在的头文件
 * @see SRE_LPUnitActive
 */
extern U32 SRE_LPUnitDeactive(U32 groupId, U32 unitNo);

/*
 * @ingroup sre_lp
 * @brief 动态加载或动态补丁段信息查询
 *
 * @par 描述
 * 查询指定LP管理组ID中的动态加载或动态补丁段基本信息。
 * @attention
 * <ul>
 * <li>只支持动态加载及动态补丁(DL&DP)。</li>
 * <li>仅SD6182/SD6157支持</li>
 * </ul>
 * @param groupId [IN]  类型#U32，注册时返回的组ID，需要用户保证其正确性。
 * @param secName [IN]  类型#char *，查询的段名。
 * @param secInfo [OUT] 类型#struct LPGroupQueryInfo *，保存段信息查询结果的指针。
 *
 * @retval #OS_ERRNO_LP_GROUP_ID_INVALID                   0x02001e72，LP管理组ID不合法。
 * @retval #OS_ERRNO_LP_MANAGE_UNIT_NOT_USED               0x02001e18，LP管理信息组未被初始化。
 * @retval #OS_ERRNO_LP_SEC_INFO_QUERY_LP_TYPE_INVALID     0x02001e25，查询动态加载或动态补丁的段信息时
 * @retval                                                             指定LP组的LP类型错误。
 * @retval #OS_ERRNO_LP_INPUT_IS_NULL                      0x02001e0d，入参指针为空。
 * @retval #SRE_OK                                         0x00000000，成功返回。
 *
 * @par 依赖
 * sre_lp.h：该接口声明所在的头文件
 * @see 无
 */
extern U32 SRE_LPSectionInfoGet(U32 groupId, const char *secName, struct LPGroupQueryInfo *secInfo);

/*
 * @ingroup sre_lp
 * @brief 已加载的处于某状态的动态加载包或补丁个数查询函数。
 *
 * @par 描述
 * 查询已加载的指定LP管理组ID中处于某状态动态加载包或补丁单元个数。
 * @attention
 * <ul><li>仅SD6182/SD6157支持</li></ul>
 *
 * @param groupId   [IN]  类型#U32，动态加载及补丁管理信息组ID，需要用户保证其正确性。
 * @param unitState [IN]  类型#enum SreLPState，单元状态，支持 UNIT_STATE_ACTIVATED(激活)，
 * UNIT_STATE_DEACTIVATED(去激活)。
 * @param unitNum   [OUT] 类型#U32 *，查询到的单元数。
 *
 * @retval #[0, 配置的最大LP单元个数]，某LP单元状态最大单元个数，可配置的最大LP单元个数为9999。
 * @retval #OS_ERRNO_LP_GROUP_ID_INVALID                   0x02001e72，LP管理组ID不合法。
 * @retval #OS_ERRNO_LP_MANAGE_UNIT_NOT_USED               0x02001e18，LP管理信息组未被初始化。
 * @retval #OS_ERRNO_LP_INPUT_IS_NULL                      0x02001e0d，入参指针为空。
 * @retval #OS_ERRNO_LP_STATE_INVALID                      0x02001e39，输入的LP单元状态不正确。
 * @retval #SRE_OK                                         0x00000000，成功返回。
 *
 * @par 依赖
 * sre_lp.h：该接口声明所在的头文件
 * @see 无
 */
extern U32 SRE_LPGetNumByState(U32 groupId, enum SreLPState unitState, U32 *unitNum);

/*
 * @ingroup sre_lp
 * @brief 已加载的动态加载单元或补丁单元个数查询函数。
 *
 * @par 描述
 * 查询已加载的指定LP管理组ID中已加载的动态加载单元或补丁单元总个数。
 * @attention
 * <ul><li>仅SD6182/SD6157支持</li></ul>
 *
 * @param groupId [IN]  类型#U32，LP管理组ID，需要用户保证其正确性。
 * @param unitNum [OUT] 类型#U32 *，查询到的单元数。
 *
 * @retval #[0, 配置的最大LP单元个数]，获取补丁单元的个数，可配置的最大LP单元个数为9999。
 * @retval #OS_ERRNO_LP_GROUP_ID_INVALID                   0x02001e72，LP管理组ID不合法。
 * @retval #OS_ERRNO_LP_MANAGE_UNIT_NOT_USED               0x02001e18，LP管理信息组未被初始化。
 * @retval #OS_ERRNO_LP_INPUT_IS_NULL                      0x02001e0d，入参指针为空。
 * @retval #SRE_OK                                         0x00000000，成功返回。
 *
 * @par 依赖
 * sre_lp.h：该接口声明所在的头文件
 * @see 无
 */
extern U32 SRE_LPGetTotalNum(U32 groupId, U32 *unitNum);

/*
 * @ingroup sre_lp
 * @brief 动态加载单元或补丁单元状态查询函数。
 *
 * @par 描述
 * 查询指定LP管理组ID中指定补丁单元或动态加载单元的状态。
 * @attention
 * <ul><li>仅SD6182/SD6157支持</li></ul>
 *
 * @param groupId      [IN]  类型#U32，注册时返回的组ID，需要用户保证其正确性。
 * @param unitNo       [IN]  类型#U32，工具端制作LP单元包时，用户指定的单元号，其取值范围[1,9999]。
 * @param unitState    [OUT] 类型#enum SreLPState *，该单元状态保存指针。
 *
 * @retval #OS_ERRNO_LP_GROUP_ID_INVALID                   0x02001e72，补丁管理组ID不合法。
 * @retval #OS_ERRNO_LP_MANAGE_UNIT_NOT_USED               0x02001e18，补丁管理信息组未被初始化。
 * @retval #OS_ERRNO_LP_INPUT_IS_NULL                      0x02001e0d，保存补丁状态的指针为空。
 * @retval #OS_ERRNO_LP_NOT_LOADED                         0x02001e09，该LP单元未加载。
 * @retval #SRE_OK                                         0x00000000，成功返回。
 *
 * @par 依赖
 * sre_lp.h：该接口声明所在的头文件
 * @see 无
 */
extern U32 SRE_LPGetUnitState(U32 groupId, U32 unitNo, enum SreLPState *unitState);

/*
 * @ingroup sre_lp
 * @brief 动态加载或补丁的升级版本信息查询函数。
 *
 * @par 描述
 * 查询指定LP管理组ID中的LP协议升级版本信息
 * (这个是工具端制作LP文件时将".o"文件打包成".pat"的协议格式版本，管理段加载".pat"文件时需要匹配该协议格式)。
 * @attention
 * <ul><li>仅SD6182/SD6157支持</li></ul>
 *
 * @param imageId      [IN]  类型#U32，静态配置的镜像ID，需要用户保证其正确性。
 * @param imageVersion [OUT] 类型#U8 *，保存版本信息查询结果指针。
 * @param length       [IN]  类型#U32，保存版本信息大小。
 *
 * @retval #OS_ERRNO_LP_GROUP_ID_INVALID                   0x02001e72，LP管理组ID不合法。
 * @retval #OS_ERRNO_LP_MANAGE_UNIT_NOT_USED               0x02001e18，LP管理信息组未被初始化。
 * @retval #OS_ERRNO_LP_INPUT_IS_NULL                      0x02001e0d，入参指针为空。
 * @retval #OS_ERRNO_LP_BUF_NOT_ENOUGH                     0x02001e0e，缓冲区长度不够。
 * @retval #OS_ERRNO_LP_IMAGE_ID_INVALID                   0x02001e2a，镜像ID错误。
 * @retval #SRE_OK                                         0x00000000，成功返回。
 *
 * @par 依赖
 * sre_lp.h：该接口声明所在的头文件
 * @see 无
 */
extern U32 SRE_LPGetVersion(U32 imageId, U8 *imageVersion, U32 length);

/*
 * @ingroup sre_lp
 * @brief 动态加载或补丁同步操作时间设置。
 *
 * @par 描述
 * 设置指定LP管理组ID中同步的超时时间（单位：ms）。
 * @attention
 * <ul>
 * <li>仅SD6182/SD6157支持</li>
 * <li>如果为配置超时时间为0，则将其设置为默认的超时时间15000ms。</li>
 * </ul>
 *
 * @param groupId  [IN]  类型#U32，注册时返回的组ID，需要用户保证其正确性。
 * @param overTime [IN]  类型#U32，同步超时时间，单位为ms。
 *
 * @retval #OS_ERRNO_LP_GROUP_ID_INVALID                   0x02001e72，LP管理组ID不合法。
 * @retval #OS_ERRNO_LP_MANAGE_UNIT_NOT_USED               0x02001e18，LP管理信息组未被初始化。
 * @retval #SRE_OK                                         0x00000000，成功返回。
 *
 * @par 依赖
 * sre_lp.h：该接口声明所在的头文件
 * @see 无
 */
extern U32 SRE_LPSetOverTime(U32 groupId, U32 overTime);

/*
 * @ingroup sre_lp
 * @brief 查询LP单元号。
 *
 * @par 描述
 * 获取指定LP文件缓冲区中的LP单元号。
 *
 * @attention
 * <ul>
 * <li>仅SD6182/SD6157支持</li>
 * <li>如果是动态加载包或者是动态补丁包，该接口获取该动态加载包或者补丁包对应的LP单元号。</li>
 * <li>如果是静态补丁包则获取该静态补丁包包含的补丁中的最大LP单元号。</li>
 * </ul>
 *
 * @param buf    [IN]  类型#U8 *，缓冲区地址。
 * @param length [IN]  类型#U32，缓冲区长度（LP文件长度）。
 * @param unitNo [OUT] 类型#U32 *，单元号。
 *
 * @retval #OS_ERRNO_LP_INPUT_IS_NULL                      0x02001e0d，动态加载或补丁缓冲区为空。
 * @retval #OS_ERRNO_LP_BUF_ADDR_NOT_ALIGN                 0x02001e22, 缓冲区地址(buf)非4字节对齐。
 * @retval #OS_ERRNO_LP_BUF_NOT_ENOUGH                     0x02001e0e，动态加载或补丁缓冲区长度不正确。
 * @retval #OS_ERRNO_LP_IDENT_INVALID                      0x02001e20，不是合法的LP文件。
 * @retval #OS_ERRNO_LP_FILE_IS_INVALID                    0x02001e16，动态加载或补丁LP文件长度不正确。
 * @retval #SRE_OK                                         0x00000000，成功返回。
 *
 * @par 依赖
 * sre_lp.h：该接口声明所在的头文件
 * @see 无
 */
extern U32 SRE_LPGetUnitNo(U8 *buf, U32 length, U32 *unitNo);

/*
 * @ingroup sre_lp
 * @brief 获取某个状态最近加载的单元号。
 *
 * @par 描述
 * 获取指定动态加载及补丁LP管理信息组ID的某个状态最迟加载的LP单元号。
 *
 * @attention
 * <li>仅SD6182/SD6157支持</li>
 *
 * @param groupId   [IN]  类型#U32，动态加载及补丁LP管理信息组ID。
 * @param unitState [IN]  类型#enum SreLPState，补丁某一个状态，支持 UNIT_STATE_ACTIVATED(激活)，
 * UNIT_STATE_DEACTIVATED(去激活)。
 * @param unitNo    [OUT] 类型#U32 *，补丁单元号。
 *
 * @retval #OS_ERRNO_LP_GROUP_ID_INVALID                   0x02001e72，LP管理组ID不合法。
 * @retval #OS_ERRNO_LP_MANAGE_UNIT_NOT_USED               0x02001e18，指定的LP管理组未初始化。
 * @retval #OS_ERRNO_LP_STATE_INVALID                      0x02001e39，输入的LP单元状态不正确。
 * @retval #SRE_OK                                         0x00000000，成功返回。
 *
 * @par 依赖
 * sre_lp.h：该接口声明所在的头文件
 * @see 无
 */
extern U32 SRE_LPGetLastUnitNo(U32 groupId, enum SreLPState unitState, U32 *unitNo);

/*
 * @ingroup sre_lp
 * @brief  根据组名查询组ID。
 *
 * @par 描述
 * 根据组名查询组ID。
 *
 * @attention
 * <li>仅SD6182/SD6157支持</li>
 *
 * @param imageId   [IN]  类型#U32，静态配置的镜像ID。
 * @param groupName [IN]  类型#U32，注册时的组名。
 * @param groupId   [OUT] 类型#U32 *，查询到的组ID。
 *
 * @retval #OS_ERRNO_LP_INPUT_IS_NULL                        0x02001e0d，动态加载或补丁缓冲区为空。
 * @retval #OS_ERRNO_LP_IMAGE_ID_INVALID                     0x02001e2a，镜像ID错误。
 * @retval #OS_ERRNO_LP_NOT_FIND_GROUP_ID                    0x02001e72，找不到指定镜像ID相应组名的组ID。
 * @retval #SRE_OK                                           0x00000000，成功返回。
 *
 * @par 依赖
 * sre_lp.h：该接口声明所在的头文件
 * @see 无
 */
extern U32 SRE_LPGroupIdGet(U32 imageId, U32 groupName, U32 *groupId);

/*
 * @ingroup sre_lp
 * @brief  获取单元的被依赖关系表。
 *
 * @par 描述
 * 查询指定LP组对应的LP外部单元号的被依赖关系表。
 *
 * @attention
 * <ul>
 * <li>仅SD6182/SD6157支持</li>
 * <li>依赖链包括自身，依赖链的顺序为从上到下，假设有依赖关系，A被B和C依赖，C被D依赖，则依赖链为DCBA。</li>
 * <li>若存储空间不够，填满后将返回错误码。</li>
 * </ul>
 *
 * @param groupID          [IN]  类型#U32 ，注册时返回的组ID。
 * @param unitNo           [IN]  类型#U32 ，工具端制作LP单元包时，用户指定的单元号。
 * @param unitNumIn        [IN]  类型#U32，依赖链的大小：能够存储的最大单元数。
 * @param dependedLink[]   [OUT] 类型#struct UnitDependedInfo ，本单元的被依赖链，数组形式。
 * @param unitNumOut       [OUT] 类型#U32* ，总的被依赖数，当依赖链空间不够时，该值将大于unitNumIn。
 *
 * @retval #OS_ERRNO_LP_INPUT_IS_NULL                        0x02001e0d，动态加载或动态补丁缓冲区为空。
 * @retval #OS_ERRNO_LP_DEPEND_LIST_NUM_IS_ZERO              0x02001e7d，调用SRE_LPUnitDependedLinkGet时发现输入的
 * @retval                                                               依赖链unitNumIn的大小为0。
 * @retval #OS_ERRNO_LP_NO_INVALID                           0x02001e01，动态加载单元号或LP单元号错误。
 * @retval #OS_ERRNO_LP_GROUP_ID_INVALID                     0x02001e72，LP管理组ID不合法。
 * @retval #OS_ERRNO_LP_MANAGE_UNIT_NOT_USED                 0x02001e18，组信息未被初始化。
 * @retval #OS_ERRNO_LP_DEPEND_LIST_NUM_IS_LITTLE            0x02001e7e，输入的依赖链unitNumIn的大小小于真实大小。
 * @retval #OS_ERRNO_LP_NOT_LOADED                           0x02001e09，该LP单元未加载。
 * @retval #OS_ERRNO_LP_MEM_NOT_ENOUGH                       0x02001e0f，内存申请失败。
 * @retval #SRE_OK                                           0x00000000，成功返回。
 *
 * @par 依赖
 * sre_lp.h：该接口声明所在的头文件
 * @see 无
 */
extern U32 SRE_LPUnitDependedLinkGet(U32 groupId, U32 unitNo, U32 unitNumIn, struct UnitDependedInfo dependedLink[],
                                     U32 *unitNumOut);

/*
 * @ingroup sre_lp
 * @brief 组注册错误上报钩子函数注册。
 *
 * @par 描述
 * 将组注册错误信息上报给用户。
 * @attention
 * <ul>
 * <li>仅SD6182/SD6157支持</li>
 * </ul>
 *
 * @param hook [IN]  类型#LPErrReportHook，保存组注册错误信息上报钩子函数类型。
 *
 * @retval #OS_ERRNO_LP_REG_ERRREPORTHOOK_NULL            0x02001e74，注册保存LP单元状态的钩子函数为空。
 * @retval #OS_ERRNO_LP_NOT_INITED                        0x02001e48，LP模块未初始化，没有经过镜像初始化。
 * @retval #SRE_OK                                        0x00000000，成功返回。
 *
 * @par 依赖
 * sre_lp.h：该接口声明所在的头文件
 * @see SRE_LPGroupInfoReg
 */
extern U32 SRE_LPErrReportHookAdd(LPErrReportHook hook);

/*
 * @ingroup sre_lp
 * @brief 完成复位通知接口。
 *
 * @par 描述
 * 单核复位完成后，调用该接口表示单核复位完成。
 * @attention
 * <ul>
 * <li>仅SD6182/SD6157支持</li>
 * <li>调用通知接口，后续针对复位核所在的组进行Fetch/Active操作时，将要求组内所有生效核参与。</li>
 * <li>调用该接口通知OS复位核已经完成重加载（含Fetch/Active）。</li>
 * <li>故障核复位后，重启成功前，不能对故障核进行动态库/动态补丁的恢复操作。</li>
 * <li>在故障核的动态库/动态补丁没有完全恢复前，调用SRE_LPCoreReloadOK，可能导致后续业务运行异常。</li>
 * </ul>
 *
 * @param 无。
 *
 * @retval 无
 *
 * @par 依赖
 * sre_lp.h：该接口声明所在的头文件
 * @see
 */
extern void SRE_LPCoreReloadOk(void);

/*
 * @ingroup sre_lp
 * @brief 加载动态库
 *
 * @par 描述
 * 加载动态库，主要是进行重定位操作
 *
 * @attention
 * <ul>
 * <li>无</li>
 * </ul>
 *
 * @param param [in]   类型#const struct DlLoadPara *，动态库加载信息用户参数。
 * @param id    [out]  类型#U32 *，动态库id。
 *
 * @retval #SRE_OK    0x00000000，成功。
 * @retval 不等于0，表示操作失败。
 *
 * @par 依赖
 * <ul><li> sre_lp.h：该接口声明所在的头文件。</li></ul>
 */
extern U32 SRE_DlInstall(const struct DlLoadPara *param, U32 *id);

/*
 * @ingroup sre_lp
 * @brief 激活动态库
 *
 * @par 描述
 * 执行动态入口函数
 *
 * @attention
 * <ul>
 * <li>无</li>
 * </ul>
 *
 * @param id [in]  类型U32，动态库id。
 *
 * @retval #SRE_OK  0x00000000，成功。
 * @retval 不等于0，表示操作失败。
 *
 * @par 依赖
 * <ul><li> sre_lp.h：该接口声明所在的头文件。</li></ul>
 */
extern U32 SRE_DlActive(U32 id);

/*
 * @ingroup sre_lp
 * @brief 去激活动态库
 *
 * @par 描述
 * 执行动态库退出函数
 *
 * @attention
 * <ul>
 * <li>无</li>
 * </ul>
 *
 * @param id [in]   类型U32，动态库id。
 *
 * @retval #SRE_OK  0x00000000，成功。
 * @retval 不等于0，表示操作失败。
 *
 * @par 依赖
 * <ul><li> sre_lp.h：该接口声明所在的头文件。</li></ul>
 */
extern U32 SRE_DlDeactive(U32 id);

/*
 * @ingroup sre_lp
 * @brief 卸载动态库
 *
 * @par 描述
 * 卸载动态库
 *
 * @attention
 * <ul>
 * <li>无</li>
 * </ul>
 *
 * @param id [in]  类型U32，动态库id。
 *
 * @retval #SRE_OK  0x00000000，成功。
 * @retval 不等于0，表示操作失败。
 *
 * @par 依赖
 * <ul><li> sre_lp.h：该接口声明所在的头文件。</li></ul>
 */
extern U32 SRE_DlUninstall(U32 id);

/*
 * @brief 加载动态补丁
 *
 * @par 描述
 * 加载动态补丁，主要是进行重定位操作
 *
 * @attention
 * <ul>
 * <li>无</li>
 * </ul>
 *
 * @param[in]  param 类型#const struct DlLoadPara *，动态补丁加载信息用户参数。
 * @param[out] id    类型#U32 *，动态补丁id。
 *
 * @retval #SRE_OK  0x00000000，成功。
 * @retval 不等于0，表示操作失败。
 *
 * @par 依赖
 * <ul><li> sre_lp.h：该接口声明所在的头文件。</li></ul>
 */
extern U32 SRE_DpInstall(const struct DlLoadPara *param, U32 *id);

/*
 * @brief 激活动态补丁
 *
 * @par 描述
 * 激活动态补丁，进行补丁函数替换
 *
 * @attention
 * <ul>
 * <li>无</li>
 * </ul>
 *
 * @param[in]  id  类型U32，动态补丁id。
 *
 * @retval #SRE_OK  0x00000000，成功。
 * @retval 不等于0，表示操作失败。
 *
 * @par 依赖
 * <ul><li> sre_lp.h：该接口声明所在的头文件。</li></ul>
 */
extern U32 SRE_DpActive(U32 id);

/*
 * @brief 去激活动态补丁
 *
 * @par 描述
 * 去激活动态补丁，还原被替换的补丁函数
 *
 * @attention
 * <ul>
 * <li>无</li>
 * </ul>
 *
 * @param[in]  id  类型U32，动态补丁id。
 *
 * @retval #SRE_OK  0x00000000，成功。
 * @retval 不等于0，表示操作失败。
 *
 * @par 依赖
 * <ul><li> sre_lp.h：该接口声明所在的头文件。</li></ul>
 */
extern U32 SRE_DpDeactive(U32 id);

/*
 * @brief 卸载动态补丁
 *
 * @par 描述
 * 卸载动态补丁
 *
 * @attention
 * <ul>
 * <li>无</li>
 * </ul>
 *
 * @param[in]  id  类型U32，动态补丁id。
 *
 * @retval #SRE_OK  0x00000000，成功。
 * @retval 不等于0，表示操作失败。
 *
 * @par 依赖
 * <ul><li> sre_lp.h：该接口声明所在的头文件。</li></ul>
 */
extern U32 SRE_DpUninstall(U32 id);

/*
 * @brief 获取动态库/补丁段信息
 *
 * @par 描述
 * 获取动态库/补丁段信息
 *
 * @attention
 * <ul>
 * <li>无</li>
 * </ul>
 *
 * @param[in]  id   类型U32，动态库/补丁id。
 * @param[in]  name 类型const char *，动态库/补丁段名。
 * @param[out] sec  类型#struct DlSecInfo *，动态库/补丁段信息。
 *
 * @retval #SRE_OK  0x00000000，成功。
 * @retval 不等于0，表示操作失败。
 *
 * @par 依赖
 * <ul><li> sre_lp.h：该接口声明所在的头文件。</li></ul>
 */
extern U32 SRE_DlGetSecInfo(U32 id, const char *name, struct DlSecInfo *sec);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* SRE_LP_H */
