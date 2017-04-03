#include "MySqlite3.h"
using namespace my_master;

int callback(void*,int,char**,char**)
{
    return 0;
}

MySqlite3::MySqlite3(const std::string dbName)
    :m_db_name(dbName),
    m_db(nullptr)
{
    m_result.clear();
    m_cur_index = 0;
}
MySqlite3::~MySqlite3()
{}

void MySqlite3::Open()
{
    int err_code = sqlite3_open(m_db_name.c_str(),&m_db);
    if(err_code != SQLITE_OK)
    {
        printf("open sqlite db fail %d\n",err_code);
    }
    ClearBuf();
}

int MySqlite3::Query(const std::string sql)
{
    char* err_msg;
    int err_code = sqlite3_exec(m_db,sql.c_str(),callback,NULL,&err_msg);
    if(err_code != SQLITE_OK)
        printf("%s\n",err_msg);
    return err_code;
}

std::vector<std::string>* MySqlite3::GetRow()
{
    if(m_result.size() < m_cur_index)
    {
        m_result.clear();
        m_cur_index = 0;
        return nullptr;
    }
    return &(m_result[m_cur_index++]);
}

void MySqlite3::ClearBuf()
{
    m_result.clear();
    m_cur_index = 0;
}

void MySqlite3::Close()
{
    if(m_db)
        sqlite3_close(m_db);
}
