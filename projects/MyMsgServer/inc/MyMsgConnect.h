#ifndef __MYCONNECT_H__
#define __MYCONNECT_H__
#include "MyMsgCommon.h"
#include "MyTcp.h"

/*!
 * \brief msg protocal
 *      build msg:
 *          BuildErr
 *      handle msg:
 *          HandleLogin
 */

class MyMsgManager;
class MyMsgConnect : public my_master::MyEasyTcpSocket
{
    friend class MyMsgManager;
public:
    MyMsgConnect(int fd, sockaddr_in addr);
    ~MyMsgConnect();
    /////////////////////////////////////////////////
    /// override parent method
    int Frame(const char* buf, int len);
    std::string GetClassType(){return m_id;}

    /////////////////////////////////////////////////
    /// Ass func
    std::vector<std::string>& GetAllFriendsId();
    MyMsgConnect* SearchMemberById(std::string id);
    void MemberQuit(std::string name);
    std::string GetAccount();
    std::string Getpass();
    void InitAccountInfo(std::string id, std::string password);
    //////////////////////////////////////////////////
    /// 消息处理
    void Handle(const char* buf, int len);
    void HandleLogin(const char* buf, int len);
    void HandleSingleMsg(const char* buf, int len);
    void HandleErr(const char* buf, int len);
    void HandleRequest(const char* buf, int len);

    //////////////////////////////////////////////////
    /// 构造消息
    char* BuildQuit(int* outlen);
    char* BuildAnswer(EnumMsgRequest_t e, char state, int *outlen);
    /// err_num : \see EnumMsgCode_t
    char* BuildErr(unsigned short err_num, int* outlen);

    //////////////////////////////////////////////////
    /// 帐号信息
    std::string m_id;               ///< id card
    std::string m_lv;               ///< is root or consumer
    std::string m_pass;
    std::string m_group;            ///< belong group
    std::string m_server;           ///< belong server
    std::string m_name;             ///< user name
private:
    std::mutex m_mutex;
    char* m_buf;                    ///< 构造消息使用的数组
    int m_buf_size;                 ///< 消息数组的大小
    bool m_isLogin;                 ///< 是否登录
    MyMap<std::string,MyMsgConnect*> m_friends;
    std::vector<std::string> m_all_friends_id;
    std::vector<AccountInfo_t> m_m_all_friends_info;
};



#endif
