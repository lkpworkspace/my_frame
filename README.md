# MyFrame
easy coding on linux 


///////////////////////////////////////
该项目使用qmake进行管理
	qmake
	make

	make clean
使用QT工具时：
	编译命令选择：
		temp.sh
	清理命令选择：
		make clean
运行时文件选择：
	TEMP/main.bin
如何使用Qt Creator打开此项目：
	使用*.pro文件打开此项目
/////////////////////////////////////////
文件的添加：
	在temp.pro文件中添加要添加的头文件以及源文件路径
	并在src文件夹中相应的文件

//////////////////////////////////////////
git管理：

SSH:
	git@github.com:lkpworkspace/my_cplusplus_master.git

push an existing repo:
	git remote add origin git@github.com:lkpworkspace/my_cplusplus_master.git
	git push -u origin master











OS:
	Ubuntu
IDE:
	QtCreator

调试笔记：
03月27号
1.调试链表程序，发现老是段错误？
	原因是我把同一个节点同时加入两个链表中，第二个链表修改该节点的prev,next指针，导致
	第一个链表段错误，切记切记！！！(一个节点在同一时刻只能在一个链表中)

2.程序加锁后，造成死锁的情况？
	在一个加锁的程序片段中，调用一个使用同样锁的函数调用，导致死锁的情况
03月28号
1.在将事件加入任务队列后没有删除导致，后续该事件有没有触发都会调用此回调函数？
	在执行过的回调函数后删除事件，是否再次加入由客户逻辑决定
2.在某个事件触发执行回调后，是否重新加入监听队列
	此版本不再加入队列，若要加入队列可在回调中加入队列
3.潜在的问题？？？
	任务有的很忙，有的很闲，需要任务调度策略
4.创建两个线程后，idle_task进入无限循环？？？
	创建两个线程后加入空闲队列，然后等待后又加入队列，造成链表混乱
	解决： 初始创建后不加入队列
	引申出的问题：
		遍历空队列时，Begin方法返回空，造成与End比较时出现程序崩溃
		解决方法：队列为空就返回 End 的值
