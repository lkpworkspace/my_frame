# ![run](https://github.com/lkpworkspace/demo/blob/master/pics/icon3.png)MyFrame
easy coding on linux

## OS&IDE
	Ubuntu
	QtCreator
## 该项目使用qmake进行管理
    qmake
    make
    make clean
## 如何使用QtCreator打开此项目
    使用*.pro文件打开此项目
## git管理
	SSH:git@github.com:lkpworkspace/my_frame.git
	push an existing repo:
		git remote add origin git@github.com:lkpworkspace/my_frame.git
		git push -u origin master

## 基于事件驱动的程序框架

|       类         |  block  |    说明                      |   继承                    | listen event|
|-----------------|:--------|:----------------------------|:-------------------------|---------------|
| MyEvent          |        |    抽象事件类                 |                          ||
| MySock           |        |    抽象套接字类               |  MyEvent                 ||
| MyUdp            |  yes   |    Udp事件类                 |  MySock                  | epollin |
| MyTcpServer      |  yes   |    Tcp事件类                 |  MySock                  | epollin |
| MyTcpClient      |  yes   |    Tcp事件类                 |  MySock                  | epollin |
| MyTcpSocket      |  no    |    Tcp事件类                 |  MyEvent                 | epollin |
| MyEasyTcpSocket  |  no    |   TCP socket                | MyTcpFrame，MyTcpSocket  | epollin |
| MyEasyTcpClient  |  no    |   TCP client                | MyTcpFrame，MyTcpClient  | epollin |
| MyNormalEvent    |  no    |    通用的事件类               | MyEvent                 | epollin |
| MyTask           |        |任务类，用于处理套接字消息       | MyEvent， MyThread       | epoolin |
| MyTimer          |        |    定时器类                  |                          ||
| MyRawSock        |        |    // TODO...               |                          ||
| MyFileEvent      |        |    // TODO...               |                          ||
| MyKeyEvent       |        |    监听键盘消息的事件类 //TODO...|                         ||
| MyMouseEvent     |        |    监听鼠标消息的事件类  //TODO...|                        ||
| MyTFTP           |        |    TFTP事件类，继承MyUdp类 //TODO...|                     ||

* block可以通过SetNonblock函数设置是否阻塞(默认为阻塞)
* 该项目使用纯C/C++以及Linux api编写，只使用QtCreator作为开发IDE
* 目前该框架主要用于网络服务器的快速开发

## 数据结构类
	MyList                           双向循环链表
	MyVec                            动态数组
	MyHeap                           堆
	MyHash                           散列表

## 调试笔记：
* MyFrame/doc/debug.md
