#ifndef __MyMsg_H__
#define __MyMsg_H__
#include <stdint.h>
#include <string>

#define USE_EASY_CLIENT

#define MYPROTO_MAX_BUF_SIZE  2048



typedef enum EnumMsgCode{
    ERR_OK,                         ///< OK
    ERR_PASSWORD,                   ///< 密码错误
    ERR_NOACCOUNT,                  ///< 帐号错误
    ERR_ALREADYLOGIN,               ///< 已经登录
    ERR_NOMEMBER,                   ///< 没有该帐号(要发送的帐号)
    ERR_NOFUNC,                     ///< 没有此功能
    ERR_FORMAT                      ///< 错误的消息格式
}EnumMsgCode_t;

typedef struct _SingleMsg
{
    std::string    src;
    std::string    dest;
    const char*    buf;
    unsigned short len;
}SingleMsg_t;

/*!
 * \brief MyMsgInit
 * \return
 */
int MyMsgInit();

/*!
 * \brief MyMsgExit
 * \return
 */
int MyMsgExit();

/*!
 * \brief MyMsgHandle
 * \param buf
 * \param len
 * \return
 */
unsigned short MyMsgHandle(const char* buf);

/*!
 * \brief MyMsgLogin
 * \param account
 * \param pass
 * \param outlen
 * \return build buffer
 */
char* MyMsgLogin(std::string account,
                 std::string pass,
                 int* outlen);

/*!
 * \brief MyMsgSingleMsg
 * \param src
 * \param dest
 * \param buf
 * \param len
 * \return build buffer
 */
char* MyMsgBuildSingleMsg(std::string src,
                     std::string dest,
                     const char* buf,
                     unsigned short len,
                     int* outlen);

int MyMsgHandleSingleMsg(const char* buf, int len, SingleMsg_t* sm);

const char* MyMsgGetErrStr(const char* buf, int len);

//char* BuildQuit(int* outlen)
//{
//    int index = 0;
//    char* quit_msg = NULL;
//    memset(m_buf,0,m_buf_size);

//    index = BuildHeader(0x0007,m_buf,m_buf_size);
//    index += BuildString(m_id.c_str(),index,m_buf,m_buf_size);
//    *outlen = index;
//    return m_buf;
//}



#endif
