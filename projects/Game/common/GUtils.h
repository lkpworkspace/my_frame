#ifndef __GUtils_H__
#define __GUtils_H__

class GMsg;
class GReplyMsg;

void GGlobalInit();

GMsg* GetGMsg();
void FreeGMsg(GMsg* inMsg);

GReplyMsg* GetGReplyMsg();
void FreeGReplyMsg(GReplyMsg* inMsg);

#endif
