#ifndef __GLoginManager_H__
#define __GLoginManager_H__

/**
 * @brief The GLoginManager class
 *
 *      account(string) + password(string)
 *
 */
class GMsg;
class MyList;

class GLoginManager : public IGMsgProcess
{
public:
    enum ELOGIN_STATE : unsigned short{
        ELS_OK,
        ELS_ACCOUNT_NOTEXIST,
        ELS_ERROR
    };

    GLoginManager()
    {}

    virtual ~GLoginManager(){}

    virtual void AppendMsg(GMsg* inMsg) override { mRecvMsgs.AddTail(inMsg); }

    virtual void ProcessMsgs() override;

    virtual MyList* GetReplyMsg() override;
protected:
    void ProcessMsg(GMsg* inMsg);
private:
    MyList mRecvMsgs;
    MyList mReplyMsgs;
};

#endif
