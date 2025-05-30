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

// enum CMD
// {
//     CMD_START = 0,
//     CMD_STOP,
//     CMD_EXIT,
// };
// struct prvmsg
// {
//     unsigned char type;
//     unsigned char cmd;
//     unsigned int size;
//     double data0;
//     double data1;
// };
// #define LEN_OF_MSG sizeof(struct prvmsg)



void ControlTaskEntry();
int rec_msg_proc(void *data, int len);
extern int send_message(unsigned char *message, int len);

#endif