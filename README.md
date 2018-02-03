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

| Event类         |  block      |    说明                     |   是否让MyAllEvent接收此事件| listen event|
|-----------------|:------------|:----------------------------|-------------------------:|---------------|
| MyEvent          |        |    抽象事件类                 |     no                   ||
| MySock           |        |    抽象套接字类，继承MyEvent   |  no                    ||
| MyUdp            |  yes   |    Udp事件类，继承MySock类    |    yes                   | epollin |
| MyTcpServer      |  yes   |    Tcp事件类，继承MySock类    |    yes                  | epollin |
| MyTcpClient      |  yes   |    Tcp事件类，继承MySock类    |    yes                  | epollin epollout |
| MyTcpSocket      |  no    |    Tcp事件类，继承MyEvent类   |    yes                  | epollin epollout |
| MyEasyTcpSocket  |  no    |   TCP socket，继承MyTcpFrame，MyTcpSocket|||
| MyEasyTcpClient  |  no    |   TCP client，继承MyTcpFrame，MyTcpClient|||
| MyNormalEvent    |  no    |    通用的事件类               |     yes                 | epollin |
| MyTimer          |  no    |    继承MyNormalEvent        | no||
| MyRawSock        |        |    // TODO...               |                          ||
| MyFileEvent      |  no    |    // TODO...               |                          ||
| MyKeyEvent       |  yes   |    监听键盘消息的事件类 //TODO...| yes                    ||
| MyMouseEvent     |  yes   |    监听鼠标消息的事件类  //TODO...      | yes                      ||
| MyTFTP           |  no    |    TFTP事件类，继承MyUdp类 //TODO...    |    no   ||

* block可以通过SetNonblock函数设置是否阻塞(默认为阻塞)
## 数据结构类
	MyList                           双向循环链表
	MyVec                            动态数组
	MyHeap                           堆
	MyHash                           散列表

## 调试笔记：
* MyFrame/doc/debug.md
