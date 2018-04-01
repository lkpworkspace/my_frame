#ifndef MyClientProxy_H
#define MyClientProxy_H

#include "MyFrame.h"

class MyClientProxy : public myframe::MyEasyTcpSocket
{
public:
    MyClientProxy(int fd, sockaddr_in addr)
        :MyEasyTcpSocket(fd, addr),
          mName(""),
          mPlayerId(-1),
          mIsLogin(false),
          mIsClientQuit(false)
    {

    }

    virtual ~MyClientProxy(){}

    /* override MyEasyTcpSocket method */
    virtual int Frame(const char* buf, int len);

    int				GetPlayerId()		const	{ return mPlayerId; }
    const	std::string&	GetName()			const	{ return mName; }
    bool IsLogin() { return mIsLogin; }
    void SetLogin(bool b) { mIsLogin = b; }
    bool IsQuit(){ return mIsClientQuit; }

    void SetPlayerId( int inPlayerId) { mPlayerId = inPlayerId; }
    void SetName(const std::string inName){ mName = inName; }

    MyReplicationManagerServer& GetReplicationManagerServer(){
        return mReplicationManagerServer;
    }

    MyList mUnProcessMsgs;
private:
    MyReplicationManagerServer mReplicationManagerServer;
    std::string mName;
    int mPlayerId;
    bool mIsLogin;
    bool mIsClientQuit;

};

#endif
