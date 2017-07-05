#include "MyICMP.h"

using namespace my_master;

MyICMP::MyICMP()
    :MySock("",0,SOCK_RAW,IPPROTO_ICMP)
{

}

MyICMP::~MyICMP()
{

}
