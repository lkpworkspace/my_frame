# 流程
1. MyClientProxy读数据流到 MyGameMsg 对象， 使用列表串起来发送到主循环线程。
2. MyNetworkManager 接收消息列表并缓存到 mPacketQueue 队列中。
3. MyNetworkManagerServer 处理接受到的消息

## 任务
* 03.25 ~ 03.31
  1. 添加回复消息管理器
  2. 完善MyPlane类的读写
  3. 循环发送MyPlane的状态
