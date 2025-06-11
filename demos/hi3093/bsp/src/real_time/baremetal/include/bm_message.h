/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2023. All rights reserved.
 */

#include "bm_types.h"
#include "bm_sharemem.h"
#ifndef _BM_MESSAGE_H_
#define _BM_MESSAGE_H_

/**
 * @defgroup bm_msg bm_msg
 */

#define MSG_PERF_STAT_ENABLE 0

typedef enum {
    MSG_STATUS_FREE = 0, // 消息buf空闲
    MSG_STATUS_USING,   // 消息buf正在被读写
    MSG_STATUS_BOTTOM,  // 无效
} msg_status_enum;

#define MAGIC_NUM 0x5a5a5a
#define msg_head_flag(status) ((status) | (MAGIC_NUM << 8))

typedef enum {
    MSG_ID_0,
    MSG_ID_1,
    MSG_ID_2,
    MSG_ID_3,
    MSG_ID_4,
    MSG_ID_BOTTOM,
} msg_id_enum;

#define HI_SHARED_IPI_ARG_LEN 0x4
#define HI_SHARED_CORE0_IPI_ARG SHARE_MSG_BASE_ADDR
#define HI_SHARED_CORE1_IPI_ARG (HI_SHARED_CORE0_IPI_ARG + HI_SHARED_IPI_ARG_LEN)
#define HI_SHARED_CORE2_IPI_ARG (HI_SHARED_CORE1_IPI_ARG + HI_SHARED_IPI_ARG_LEN)
#define HI_SHARED_CORE3_IPI_ARG (HI_SHARED_CORE2_IPI_ARG + HI_SHARED_IPI_ARG_LEN)

#define MSG_HEAD_LEN 4
#define MSG_MEM_BEGIN (HI_SHARED_CORE3_IPI_ARG + HI_SHARED_IPI_ARG_LEN)

#define MSG0_BASE MSG_MEM_BEGIN
#define MSG0_BODY_LEN 0x100
#define MSG1_BASE (MSG0_BASE + MSG0_BODY_LEN + MSG_HEAD_LEN)
#define MSG1_BODY_LEN 0x100
#define MSG2_BASE (MSG1_BASE + MSG1_BODY_LEN + MSG_HEAD_LEN)
#define MSG2_BODY_LEN 0x100
#define MSG3_BASE (MSG2_BASE + MSG2_BODY_LEN + MSG_HEAD_LEN)
#define MSG3_BODY_LEN 0x100
#define MSG4_BASE (MSG3_BASE + MSG3_BODY_LEN + MSG_HEAD_LEN)
#define MSG4_BODY_LEN 0x100

#define MSG_ID_MEM_END   (MSG4_BASE + MSG4_BODY_LEN + MSG_HEAD_LEN)

#define HI_SHARED_IPI_PERF_LEN 0x8
#define HI_SHARED_IPI_PERF_BUF MSG_ID_MEM_END

#define MSG_MEM_END (HI_SHARED_IPI_PERF_BUF + HI_SHARED_IPI_PERF_LEN)
#if (MSG_MEM_END >= SHARE_MSG_END_ADDR)
    #error "SHARE_MSG_LEN too small"
#endif

/**
 *
 * @ingroup bm_msg
 * @brief Obtains the message status.
 * @par description: Obtains the message status.
 * @param msg_id[in]:  message ID (see msg_id_enum).
 * @param *status[out]: Message status
 * @return void
 */
void bm_get_msg_status(unsigned int msg_id, msg_status_enum* status);
/**
 *
 * @ingroup bm_msg
 * @brief Setting the Message Status.
 * @par description: Setting the Message Status
 * @param msg_id[in]:  message ID (see msg_id_enum).
 * @param status[in]: Message status
 * @return void
 */
void bm_set_msg_status(unsigned int msg_id, msg_status_enum status);
/**
 *
 * @ingroup bm_msg
 * @brief Compare and set message status, atomic operation
 * @par description: The interface compares the message status of msg_id with the old value. If they are the same, the
 * interface sets the message status of msg_id to val and returns 0. Otherwise, the interface does not perform any
 * operation and returns a non-zero value.
 * @param msg_id[in]:  message ID (see msg_id_enum).
 * @param val[in]: Message status to set
 * @param old[in]: old Message status
 * @return 0/non-zero
 */
int bm_cmp_xchg_status(unsigned int msg_id, msg_status_enum val, msg_status_enum old);

/**
 *
 * @ingroup bm_msg
 * @brief Message sending interface
 * @par description: Copy the message to the buffer corresponding to msg_id.
 * @param msg_id[in]:  message ID (see msg_id_enum)
 * @param buf[in]: Pointer to the buffer of the message to be sent
 * @param buf_len[in]: Length of the buf, The value must be less than the length of msgid.

 * @return BM_OK/BM_FAIL
 */
unsigned int bm_send_msg(unsigned int msg_id, unsigned char* buf, unsigned int buf_len);
/**
 *
 * @ingroup bm_msg
 * @brief Send a message and notify the receiving core of the IPI.
 * @par description: Copy messages to the buffer corresponding to msg_id and trigger the IPI interrupt of the receive
 * core.
 * @param recv_cpu[in]: IPI interrupt target core
 * @param msg_id[in]:  message ID (see msg_id_enum)
 * @param buf[in]: Pointer to the buffer of the message to be sent
 * @param buf_len[in]: Length of the buf, The value must be less than the length of msgid.
 * @return BM_OK/BM_FAIL
 */
unsigned int bm_send_msg_with_ipi(unsigned int recv_cpu, unsigned int msg_id, unsigned char* buf, unsigned int buf_len);
/**
 *
 * @ingroup bm_msg
 * @brief Obtain the memory pointer of the msg_id message.
 * @par description: Obtain the memory pointer of the msg_id message.
 * @param msg_id[in]:  message ID (see msg_id_enum)
 * @return Message buffer addr
 */
unsigned char* bm_get_msg(unsigned int msg_id);
/**
*
* @ingroup bm_msg
* @brief Obtain the message buffer addr from the IPI interrupt ISR.
* @par description: Obtain the buffer pointer of the corresponding message that triggers the IPI interrupt from the IPI
interrupt ISR.
* @return Message buffer addr
*/
unsigned char *bm_get_ipi_msg(void);
/**
*
* @ingroup bm_msg
* @brief Message receiving interface
* @par description: This interface is used to receive messages and copy messages of msg_id to the buffer.
* @param msg_id[in]:  message ID (see msg_id_enum)
* @param buf[in]: Pointer to the buffer of the message to be received
* @param buf_len[in]: Length of buf, the value cannot be less than the length of msgid
* @return BM_OK/BM_FAIL
*/
unsigned int bm_recv_msg(unsigned int msg_id, unsigned char* buf, unsigned int buf_len);
/**
 *
 * @ingroup bm_msg
 * @brief Interface for the IPI to interrupt message receiving in the ISR
 * @par description: This interface is used to receive messages in the IPI interrupt ISR. The corresponding message that
 * triggers the IPI interrupt is copied to the buffer, and the output parameter msg_id is returned.
 * @param msg_id[out]:  message ID (see msg_id_enum)
 * @param buf[in]: Pointer to the buffer of the message to be received
 * @param buf_len[in]: Length of buf, the value cannot be less than the length of msgid
 * @return BM_OK/BM_FAIL
 */
unsigned int bm_recv_msg_with_ipi(unsigned int* msg_id, unsigned char* buf, unsigned int buf_len);

/**
 *
 * @ingroup bm_msg
 * @brief show each ipi msg cycle cost
 * @return void
 */
void bm_msg_show_perf(void);

#endif // _BM_MESSAGE_H_
