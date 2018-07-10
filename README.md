# ![run](https://github.com/lkpworkspace/demo/blob/master/pics/icon3.png)MyFrame
easy coding on linux

## OS&IDE&Language
	Ubuntu
	QtCreator
	C++
	
## Build Projects

* Terminal  
  qmake  
  make  
  make clean  

* Qtcreater  
	open QtCreator IDE whth \*.pro file  
	build

## Git
	SSH:git@github.com:lkpworkspace/my_frame.git
	push an existing repo:
		git remote add origin git@github.com:lkpworkspace/my_frame.git
		git push -u origin master

##  message-based and event-driven application framework

* 该项目使用纯C/C++以及Linux api编写，只使用QtCreator作为开发IDE
* 目前该框架主要用于网络服务器的快速开发
* Support:
	- 使用线程池实现的应用程序框架，支持高并发
	- 基于事件驱动的程序框架
	- 支持线程间无锁通信，提高并发性能
	- 支持消息被指定线程处理
	- 支持并行运算
	- 支持消息嵌套
	- 客户端支持Linux/Windows平台
	- 框架集成TCP/UDP，可以快速开发网络服务器
	- 支持事件扩展，通过重写实现MyEvent类中的虚函数实现(eg: mouse event， keyboard event，hotplug event...)

* Defect:
	- Only support linux
	- No document (you can find more docs in the code)

## class summary

|       class     |  block  |    manual                     |   inherit                | listen event|
|-----------------|:--------|:------------------------------|:-------------------------|---------------|
| MyEvent         |        |    abstract class             |                          ||
| MySock          |        |    abstract socket class      |  MyEvent                 ||
| MyUdp           |  yes   |    UDP class                  |  MySock                  | epollin |
| MyTcpServer     |  yes   |    TCP Server class           |  MySock                  | epollin |
| MyTcpClient     |  yes   |    TCP Client class           |  MySock                  | epollin |
| MyTcpSocket     |  no    |    TCP Clientproxy class      |  MyEvent                 | epollin |
| MyEasyTcpSocket |  no    |    Easy TCP Clientproxy class |  MyTcpSocket             | epollin |
| MyEasyTcpClient |  no    |    Easy TCP Client class      |  MyTcpClient             | epollin |
| MyNormalEvent   |  no    |    Normal event class         |  MyEvent                 | epollin |
| MyTask          |        |    Process event class        |  MyEvent MyThread        | epoolin |
| MyTimer         |        |    Timer class                |                          ||
| MyMsgPool       |        |    Message class manager      |                          ||
| MyApp           |        |    Main loop class            |                          ||
| MyIOStream      |        |    Build Buf class            |                          ||
| MyLog           |        |    Log class                  |                          ||
| MyRawSock       |        |    //TODO...                  |                          ||
| MyFileEvent     |        |    //TODO...                  |                          ||
| MyKeyEvent      |        |    //TODO...                  |                          ||
| MyMouseEvent    |        |    //TODO...                  |                          ||
| MyTFTP          |        |    //TODO...                  |                          ||

* block (default: nonblock)

## data struct class
	MyList                           double link list
	MyVec                            vector
	MyHeap                           heap
	MyHash                           hash table

## debug note：
* MyFrame/doc/debug.md
