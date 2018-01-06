#ifndef __MyMsg_H__
#define __MyMsg_H__
#include <stdint.h>
#include <string>

#define USE_EASY_CLIENT

#define MYPROTO_MAX_BUF_SIZE  2048

typedef enum _EnumMsgHead{
    HD_SIGMSG = 0x0009,
    HD_ANSWER = 0xffff,
}EnumMsgHead_t;

typedef enum _EnumMsgCode{
    ERR_OK,                         ///< OK
    ERR_PASSWORD,                   ///< 密码错误
    ERR_NOACCOUNT,                  ///< 帐号错误
    ERR_ALREADYLOGIN,               ///< 已经登录
    ERR_NOMEMBER,                   ///< 没有该帐号(要发送的帐号)
    ERR_NOFUNC,                     ///< 没有此功能
    ERR_FORMAT                      ///< 错误的消息格式
}EnumMsgCode_t;

typedef enum _EnumMsgRequest{
    REQ_NONE,
    REQ_LOGIN,
    REQ_SINMSG,
    REQ_ALLFRIEND,
    REQ_ONLINEFRIEND,
    REQ_SEARCHFRIENDID
}EnumMsgRequest_t;

typedef struct _MsgAnswer{
    EnumMsgRequest_t request;
    EnumMsgCode_t    err_code;
}MsgAnswer_t;

typedef struct _SingleMsg
{
    std::string    src;
    std::string    dest;
    const char*    buf;
    unsigned short len;
}SingleMsg_t;


int MyMsgInit();
int MyMsgExit();

const char* MyMsgGetRequest(EnumMsgRequest_t emr);
const char* MyMsgGetErr(EnumMsgCode_t emc);


unsigned short MyMsgHandle(const char* buf);


char* MyMsgLogin(std::string account,
                 std::string pass,
                 int* outlen);

char* MyMsgBuildSingleMsg(std::string src,
                     std::string dest,
                     const char* buf,
                     unsigned short len,
                     int* outlen);
int MyMsgHandleSingleMsg(const char* buf, int len, SingleMsg_t* sm);


const char* MyMsgBuildRequest(EnumMsgRequest_t re, int *out_len);
int MyMsgHandleAnswer(MsgAnswer_t *answer, const char* buf, int len);


#endif
