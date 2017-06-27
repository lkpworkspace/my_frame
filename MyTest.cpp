#include "MyTest.h"

std::set<void*> MyTest::l_members;

MyTest::MyTest()
{}


void MyTest::Add(void* m)
{
    if(Has(m)) return;
    l_members.insert(m);
}

bool MyTest::Has(void* m)
{
    if(l_members.find(m) != l_members.end())
        return true;
    return false;
}

void MyTest::Del(void* m)
{
    l_members.erase(m);
}
