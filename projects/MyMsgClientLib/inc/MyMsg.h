#ifndef __MyMsg_H__
#define __MyMsg_H__
#include <stdint.h>
#include <string>

#define USE_HEAD

#define MYPROTO_MAX_BUF_SIZE  2048

typedef struct data_t
{
    uint16_t len;
    char buf[MYPROTO_MAX_BUF_SIZE];
}data_t;

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
char* MyMsgSingleMsg(std::string src,
                     std::string dest,
                     const char* buf,
                     unsigned short len,
                     int* outlen);








#endif
