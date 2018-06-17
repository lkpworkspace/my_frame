#ifndef __GMatchingManager_H__
#define __GMatchingManager_H__

class GMsg;
class MyList;
/**
 * @brief The GMatchingManager class
 *      matching action(2byte)
 */
class GMatchingManager : public IGMsgProcess
{
public:
    enum EMATCHING_ACTION : unsigned short{
        EMA_MATCHING,
        EMA_CANCEL,
    };

    virtual void AppendMsg(GMsg* inMsg) override { mRecvMsgs.AddTail(inMsg); }

    /* call by GNetManager class */
    virtual void ProcessMsgs() override;

    /* call by replication class */
    virtual MyList* GetReplyMsg() override { return &mReplicationMsg; }

protected:
    void ProcessMsg(GMsg* inMsg);

    void MatchingRule();

    GReplyMsg *BuildCreateSceneMsg(int* inPlayers);
private:
    MyList mRecvMsgs;
    MyList mReplicationMsg;
    /* player id list */
    std::list<int> mMatchingList;
};

#endif
