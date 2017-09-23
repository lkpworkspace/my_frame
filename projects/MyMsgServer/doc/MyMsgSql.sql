#创建MyMsgAccount表
CREATE TABLE MyMsgAccount(
	account TEXT NOT NULL PRIMARY KEY, 
	password TEXT NOT NULL, 
	name TEXT NOT NULL, 
	msg_group TEXT NOT NULL, 
	msg_lv TEXT NOT NULL,
	msg_server TEXT NOT NULL
);
