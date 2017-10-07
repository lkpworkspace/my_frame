#ifndef __MyMsg_H__
#define __MyMsg_H__
#include <stdint.h>

#define MYPROTO_MAX_BUF_SIZE  2048
typedef struct data_t
{
    uint16_t len;
    char buf[MYPROTO_MAX_BUF_SIZE];
}data_t;


int MyMsgInit();
int MyMsgExit();















#endif
