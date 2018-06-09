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
        :mNewPlayerId(0)
    {}

    virtual ~GLoginManager(){}

    virtual void ProcessFunc(GMsg* inMsg) override;

    virtual MyList* GetReplyMsg() override;

private:
    int mNewPlayerId;
    MyList mReplyMsgs;
};

#endif
