#include <iostream>
#include "MyFrame.h"
#include "MyUdp.h"
#include "MySqlite3.h"
#include "MyTalkServer.h"

using namespace std;
using namespace my_master;
MyUdp *g_udp = nullptr;
MySqlite3* g_db = nullptr;

int main(int argc, char *argv[])
{
    argc = argc;
    argv = argv;
    // init my database
    g_db = new MySqlite3("../mytalk.db");
    g_db->Open();
#if 0 // create table
    std::string sql;
    sql = "CREATE TABLE member(account TEXT NOT NULL, name TEXT NOT NULL, password TEXT NOT NULL, mark TEXT)";
    g_db->ExecSql(sql.c_str());
    sql = "CREATE TABLE friend(account TEXT NOT NULL, belong TEXT, friend TEXT NOT NULL)";
    g_db->ExecSql(sql.c_str());
#endif
#if 0 // insert data
    std::string sql;
    sql = "insert into member values('kpli','kpli','123456','easy coding on linux')";
    g_db->ExecSql(sql.c_str());
    sql = "insert into member values('xiaoming','xiaoming','123456','easy coding on linux')";
    g_db->ExecSql(sql.c_str());
#endif
#if 0
    g_db->Close();
#endif
#if 0
    std::string sql;
    sql += "select * from member where account = '";
    sql += "kpli";
    sql += "' and password = '";
    sql += "123456";
    sql += "'";
    g_db->ExecSql(sql.c_str());
#endif
    // init my frame
    MyApp app(2,1024);

    g_udp = new MyUdp("",4399,true);
    g_udp->Bind();
    app.AddEvent(g_udp);

    // init my talk server
    MyTalkServer* server = new MyTalkServer();

    // run
    return app.Exec();
}

#if 0
ALTER TABLE的SQLite SQLite支持ALTER TABLE的一个有限子集。在ALTER SQLite中允许一个表或一个新列添加到现有表。删除列，或者添加或从表中删除约束。sqlite中是不支持删除有值的列操作的，所以alter table table_name drop column col_name这个语句在sqlite中是无效的，而替代的方法可以如下：
1.根据原表创建一张新表
2.删除原表
3.将新表重名为旧表的名称
示例例子如下
1.创建一张旧表Student，包含id（主码），name, tel
create table student (
id integer primary key,
name text,
tel text
)
2.给旧表插入两个值
insert into student(id,name,tel) values(101,"Jack","110")
insert into student(id,name,tel) values(102,"Rose","119")
结果如图
3.接下来我们删除电话这个列，首先根据student表创建一张新表teacher
create table teacher as select id,name from student
4.然后我们删除student这个表
drop table if exists student
5.将teacher这个表重命名为student
alter table teacher rename to student
结果演示：
select * from student order by name desc（desc降序， asc升序）


1、支持多表连接，例如
select * from student,class
where student.cid=class.id;

2、支持左外连接(left outer join)
例如：
select * from foods
left outer join food_types
on foods.id=food_types.food_id

// sql
select * from friend;
select * from member;

select  member.* from member, friend where member.account=friend.friend and friend.account='kpli';

insert into member values('xiaoming','xiaoming','123456','easy coding on linux');
insert into friend values('kpli','root','xiaoming');
insert into friend values('xiaoming','root','kpli');
#endif
