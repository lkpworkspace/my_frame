#ifndef __GMsgType_H__
#define __GMsgType_H__

enum EMSG_TYPE : unsigned short
{
    /* 登录请求/回复 */
    EMT_LOGIN,
    /* 匹配请求/回复 */
    EMT_MATCHING,
    /* 客户端输入 */
    EMT_INPUT,
    /* 世界状态 */
    EMT_WORLD_STATE,
    /* 远程过程调用 */
    EMT_RPC
};


#endif
