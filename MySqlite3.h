#ifndef MYSQLITE3_H
#define MYSQLITE3_H
#include "Common.h"
#include "3rd_src/Sqlite3/sqlite3.h"
#include <vector>
namespace my_master {
typedef std::vector<std::string> row_t;
class MySqlite3
{
public:
    MySqlite3(const std::string dbName);
    ~MySqlite3();

    void Open();
    int ExecSql(const std::string sql);
    std::vector<std::string>* GetRow(); // hang
    const std::string GetValue(int row, int col);
    std::vector<std::string>* GetColumnName(){ return &m_col_name; }
    int GetColCount(){ return m_col_count; }
    void ClearBuf();
    void Close();
private:
    static int sqlite_callback(void *,int ,char **,char **);
    sqlite3* m_db;
    std::string m_db_name;

    std::vector<std::string> m_col_name;
    std::vector<row_t> m_result;
    int m_cur_index;
    int m_col_count;
    int m_row_count;
    bool m_is_save_name;
};


}
#endif // MYSQLITE3_H
