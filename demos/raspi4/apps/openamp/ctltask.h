#ifndef __CTL_TASK_H
#define __CTL_TASK_H

#include "filter.h"

enum TYPE
{
    MSG_CONTROL_START = 0,
    MSG_CONTROL_STOP,
    MSG_CONTROL_EXIT,
    MSG_INPUTDATA,
    MSG_STEP,
    MSG_OUTPUT,
};

#define BUFF_LEN 100
#define MSG_FORMAT "%d %d %lf %lf\r\n"


void ControlTaskEntry();
int rec_msg_proc(void *data, int len);
extern int send_message(unsigned char *message, int len);

#endif