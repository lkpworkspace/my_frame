#ifndef MYICMP_H
#define MYICMP_H
#include "Common.h"
#include "MyNet.h"
#include "MySock.h"


namespace my_master {

class MyICMP : public my_master::MySock
{
public:
    MyICMP();
    ~MyICMP();
    ////////////////////////////////////////////////
    /// override parent method
    virtual void* CallBackFunc(MyEvent *){}
    virtual std::string GetClassType(){return "MyICMP";}

    int Read(){return 0;}
    int Write(){return 0;}
};

} // end namespace
#endif // MYICMP_H
