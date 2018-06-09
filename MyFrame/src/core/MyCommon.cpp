#include "MyCommon.h"
#include <unordered_map>


static std::unordered_map<std::string, int> g_task_id;

// TODO(lkp)...
int GetTaskIdentify(const char* inTaskName)
{
    if(g_task_id.find(inTaskName) == g_task_id.end())
        return -1;
    return g_task_id[inTaskName];
}

void SetTaskIdentifyWithName(const char* inTaskName, int inTaskId)
{
    g_task_id.emplace(inTaskName, inTaskId);
}








