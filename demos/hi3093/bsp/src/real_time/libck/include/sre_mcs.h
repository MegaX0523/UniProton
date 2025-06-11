/*
 * Copyright (c) HiSilicon (Shanghai) Technologies Co., Ltd. 2023-2023. All rights reserved.
 */

#ifndef __SRE_MCS_H__
#define __SRE_MCS_H__

#include "openamp/open_amp.h"

/**
* @defgroup sre_mcs sre_mcs
*/

/**
*
* @ingroup sre_mcs
* @brief mcs create endpoint.
*
* @par description:
* mcs create endpoint.
*
* @param ep_name [in]: endpoint name
* @param cb      [in]: endpoint callback
*
* @return OS_OK/OS_ERROR
*
*/
int sre_mcs_create_ept(const char *ep_name, rpmsg_ept_cb cb);

/**
*
* @ingroup sre_mcs
* @brief mcs destroy endpoint.
*
* @par description:
* mcs destroy endpoint.
*
* @param ep_name [in]: endpoint name
*
* @return OS_OK/OS_ERROR
*
*/
int sre_mcs_destroy_ept(const char *ep_name);

/**
*
* @ingroup sre_mcs
* @brief mcs send data.
*
* @par description:
* mcs send data.
*
* @param ep_name [in]: endpoint name
* @param data    [in]: data pointer
* @param len     [in]: data length
*
* @return success send length/other FAIL
*
*/
int sre_mcs_send(const char* ep_name, const void *data, int len);

/**
*
* @ingroup sre_mcs
* @brief mcs get zero copy data addr.
*
* @par description:
* mcs get zero copy data addr.
*
* @param ep_name    [in]: endpoint name
* @param buffer_len [in]: buffer length
*
* @return BUFFER ADDR/NULL
*
*/
void* sre_mcs_get_zero_cpy_data_addr(const char* ep_name, int* buffer_len);

/**
*
* @ingroup sre_mcs
* @brief mcs shutdown.
*
* @par description:
* mcs shutdown power.
* @return void
*
*/
void sre_mcs_shutdown(void);

#endif /* __SRE_MCS_H__ */