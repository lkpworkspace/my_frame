#ifndef __GPRCManager_H__
#define __GRPCManager_H__

class GMsg;
class MyList;

typedef void (*RPCFunc_t)(GMsg* inMsg, GMsg* outMsg);

/**
 * @brief The GRPCManager class
 *
 *      func name + func arg
 */
class GRPCManager : public IGMsgProcess
{
public:
    enum EPRC_ARG_TYPE : unsigned short{
        ERPC_ARG_CAHR,   /* char */
        ERPC_ARG_UCAHR,  /* unsigned char */
        ERPC_ARG_SHORT,  /* short */
        ERPC_ARG_USHORT, /* unsigned short */
        ERPC_ARG_INT,    /* int */
        ERPC_ARG_UINT,   /* unsigned int */
        ERPC_ARG_FLOAT,  /* float */
        ERPC_ARG_DOUBLE, /* double */
        ERPC_ARG_STRING, /* len + string */
        ERPC_ARG_RAWDATA,/* len + data */
        EPRC_ARG_STRUCT, /* len + STRING + data */
    };

    /* call by GNetManager class */
    virtual void ProcessFunc(GMsg* inMsg) override;

    /* call by replication class */
    virtual MyList* GetReplyMsg() override { return &mReplicationMsg; }

    /* Register func by name */
    bool RegRPCFunc(std::string inFuncName, RPCFunc_t inFunc);
private:
    std::unordered_map<std::string,RPCFunc_t> mRPCFuncsMap;
    MyList mReplicationMsg;
};

#endif
