#ifndef __GReply_H__
#define __GReply_H__

/**
 * @brief The GReply class
 *      写线程
 */
class GReply : public MyTask
{
public:
    GReply();
    virtual ~GReply(){}
    ///////////////////////////////////////////////// 可以重写的函数
    /**
     * 线程初始化函数，仅初始化调用一次，
     * note that: 需要调用父类中的方法
     */
    //virtual void OnInit() override;

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
