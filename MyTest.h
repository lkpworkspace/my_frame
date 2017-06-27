#ifndef MYTEST_H
#define MYTEST_H
#include <set>

class MyTest
{
public:
    MyTest();

    static void Add(void*);
    static bool Has(void*);
    static void Del(void*);

private:
    static std::set<void*> l_members;
};

#endif // MYTEST_H
