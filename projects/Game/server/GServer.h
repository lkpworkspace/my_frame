#ifndef __GServer_H__
#define __GServer_H__

#include "MyFrame.h"

#include "GCommon.h"



#include "GClientProxy.h"
#include "GReply.h"
#include "GScene.h"

#include "GRPCManager.h"
#include "GLoginManager.h"
#include "GMatchingManager.h"
#include "GSceneManager.h"
/**
 * @brief The GServer class
 *      主要的游戏运行线程
 *          目前使用单线程运行所有的游戏场景(2018-6-6 还没有明确需求要使用多线程， 等有需求再进行扩展)
 *      功能概述：
 *          接受来自客户端的消息，根据消息的类型进行分发或者进行相应的动作
 *              1. 接收到开始一个场景的动作，就创建一个场景，然后去运行一个场景
 *              2. 接收到客户端的消息，就将客户端的消息分发到对应的场景中去
 *          消息传递：
 *              1. 线程间传递消息
 *                  使用线程间传递消息，可以使接收消息线程快速返回处理其他消息，
 *                  另外这样可以避免消息处理
 *          消息格式：
 *              msg_type(unsigned short) + ...
 */
class GServer : public MyTask
{
public:
    GServer();
    virtual ~GServer(){}

    ///////////////////////////////////////////////// 可以重写的函数
    /**
     * 线程初始化函数，仅初始化调用一次，
     * note that: 需要调用父类中的方法
     */
    virtual void OnInit() override;

    /**
     * 线程退出函数，仅退出调用一次，
     * note that: 需要调用父类中的方法
     */
    //virtual void OnExit() override;

    /**
     * 每次线程收到事件便会调用此函数，如果设置线程为无限循环的(SetLoop)，
     * 则不管有没有事件该线程会一直调用此函数，
     * 所以需要在Update中加一点延时
     */
    virtual void Update(MyList *evs) override;
private:

};

#endif
