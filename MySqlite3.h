#ifndef MYSQLITE3_H
#define MYSQLITE3_H
#include "Common.h"
#include "3rd_src/Sqlite3/sqlite3.h"
namespace my_master {

class MySqlite3
{
    typedef std::vector<std::string> row_t;
public:
    MySqlite3(const std::string dbName);
    ~MySqlite3();

    void Open();
    int Query(const std::string sql);
    std::vector<std::string>* GetRow(); // hang
    void ClearBuf();
    void Close();
private:
    sqlite3* m_db;
    std::string m_db_name;

    std::vector<row_t> m_result;
    int m_cur_index;
};


}
#endif // MYSQLITE3_H
