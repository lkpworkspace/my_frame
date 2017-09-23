#include "../inc/MySqlite3.h"
using namespace my_master;

int MySqlite3::sqlite_callback(void *params,int column_size,
                    char **column_value,
                    char **column_name)
{
#if 0
    // 一次只有一条记录
    int i;
    printf("记录包含 %d 个字段\n", column_size);
    for (i = 0; i < column_size; i++)
        printf("字段名:%s 字段值:%s\n", column_name[i], column_value[i]);
    printf("------------------\n");
#endif
    MySqlite3* sqlite = (MySqlite3*)params;
    std::vector<row_t>* result = &(sqlite->m_result);
    row_t row;
    int i;

    for (i = 0; i < column_size; i++)
    {
        std::string temp(column_value[i]);
        row.push_back(temp);
        if(!sqlite->m_is_save_name)
        {
            std::string temp(column_name[i]);
            std::vector<std::string> *name = &(sqlite->m_col_name);
            name->push_back(temp);
        }
    }
    result->push_back(row);
    sqlite->m_col_count = column_size;
    sqlite->m_is_save_name = true;
    sqlite->m_row_count++;
    return 0;
}

MySqlite3::MySqlite3(const std::string dbName)
    :m_db_name(dbName),
    m_db(nullptr)
{
    ClearBuf();
}
MySqlite3::~MySqlite3()
{
    Close();
}

void MySqlite3::Open()
{
    int err_code = sqlite3_open(m_db_name.c_str(),&m_db);
    if(err_code != SQLITE_OK)
    {
        printf("open sqlite db fail %d\n",err_code);
    }
}

void MySqlite3::EventBegin()
{
    int err_code;
    if((err_code = sqlite3_exec(m_db,"begin;",0,0,0)) != SQLITE_OK)
        printf("[MySqlite3::EventBegin] : exec sql fail %d\n",err_code);
}

void MySqlite3::EventEnd()
{
    int err_code;
    if((err_code = sqlite3_exec(m_db,"commit;",0,0,0)) != SQLITE_OK)
        printf("[MySqlite3::EventEnd] : exec sql fail %d\n",err_code);
}

int MySqlite3::ExecSql(const std::string sql)
{
    char* err_msg;

    ClearBuf();
    int err_code = sqlite3_exec(m_db,sql.c_str(),sqlite_callback,this,&err_msg);
    if(err_code != SQLITE_OK)
        printf("%s\n",err_msg);
    return err_code;
}

std::vector<std::string>* MySqlite3::GetRow()
{
    if(m_result.size() <= m_cur_index)
    {
        return nullptr;
    }
    return &(m_result[m_cur_index++]);
}

const std::string MySqlite3::GetValue(int row, int col)
{
    if(row >= m_row_count || col >= m_col_count)return "";
    return m_result[row][col];
}

void MySqlite3::ClearBuf()
{
    m_result.clear();
    m_col_name.clear();
    m_col_count = 0;
    m_cur_index = 0;
    m_row_count = 0;
    m_is_save_name = false;
}

void MySqlite3::Close()
{
    if(m_db)
        sqlite3_close(m_db);
    ClearBuf();
}



#if 0
    int sqlite3_exec(
        sqlite3*,                                  /* An open database */
        const char *sql,                           /* SQL to be evaluated */
        int(*callback)(void*, int, char**, char**),  /* Callback function */
        void *,                                    /* 1st argument to callback */
        char **errmsg                              /* Error msg written here */
        );
    各个参数的意义为：
        sqlite3描述的是数据库句柄
        sql 要执行的SQL语句
        callback回调函数
        void *回调函数的第一个参数
        errmsg错误信息，如果没有SQL问题则值为NULL

        int callback(void *params,
        int column_size,
        char **column_value,
        char **column_name);
    各个参数的意义为：
        params是sqlite3_exec传入的第四个参数
        column_size是结果字段的个数
        column_value是返回记录的一位字符数组指针
        column_name是结果字段的名称
#endif




























