#ifndef __MySqlite3_test_h__
#define __MySqlite3_test_h__
#include "MyFrame.h"
#include "MyTest.h"
#include "MySqlite3.h"

class MySqlite3_test
{
public:
    MySqlite3_test()
    {

    }

    void test()
    {
#if 1
    MySqlite3 db("mydb.db");
    db.Open();
    //db.ExecSql("create table test(id, name, age)");
#if 0
    for(int i = 0; i < 10; ++i)
        db.ExecSql("insert into test values(10,'haha',20)");
#else
    db.ExecSql("select * from test");
#if 0
    while(true)
    {
        std::vector<std::string> *row = db.GetRow();
        if(row)
        {
            for(int i = 0; i < row->size(); ++i)
            {
                std::cout <<(*row)[i] << " ";
                //(*db.GetColumnName())[i];
            }
            std::cout << std::endl;
        }
        else
        {
            std::cout << "no data"  << std::endl;
            break;
        }
    }
    for(int i = 0; i < db.GetColCount(); ++i)
    {
        std::cout << (*db.GetColumnName())[i] << std::endl;
    }
#else
    std::cout << db.GetValue(0,2);
#endif
#endif
    db.Close();
#endif
    }
};




#endif
