# 创建MyMsgAccount表
CREATE TABLE MyMsgAccount(
	account TEXT NOT NULL PRIMARY KEY, 
	password TEXT NOT NULL, 
	name TEXT NOT NULL, 
	msg_group TEXT NOT NULL, 
	msg_lv TEXT NOT NULL,
	msg_server TEXT NOT NULL
);

# 创建好友关系表
CREATE TABLE MyMsgFriends(
    account TEXT NOT NULL, 
    friend TEXT NOT NULL,
    foreign key(account) references MyMsgAccount()
);

# 查询帐号信息
select * from MyMsgAccount where account="123456789"

# 查询某个帐号的好友信息
select * from MyMsgFriends where account = "123456789"

# 查询存在的服务器
select distinct msg_server from mymsgaccount

# 查询0服务器下所有组
select distinct msg_group from mymsgaccount where msg_server=‘0’

# 查询指定服务器下的组的所有成员
select * from mymsgaccount where msg_server='0' and msg_group='0'


