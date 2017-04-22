#include <iostream>
#include <stdint.h>
#include <stdio.h>

#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <mqueue.h>

/*send.c*/
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#define MSGKEY 1024
struct msgstru
{
   long msgtype;
   char msgtext[1024];
};

using namespace std;

int main(int argc, char *argv[])
{
#if 0
    mqd_t msg;
    msg = mq_open("/my_frame_msg_queue",O_RDWR);

    char buf[10] = "hello";
    char ch;
    while((ch = getchar()) != 'q')
    {
        mq_send(msg, buf, 5, NULL);
    }

    mq_close(msg);
    mq_unlink("/my_frame_msg_queue");
#else
      struct msgstru msgs;
      int msg_type;
      char str[256];
      int ret_value;
      int msqid;

      msqid=msgget(MSGKEY,IPC_EXCL);  /*检查消息队列是否存在*/
      if(msqid < 0){
#if 0
        msqid = msgget(MSGKEY,IPC_CREAT|0666);/*创建消息队列*/
        if(msqid <0){
        printf("failed to create msq | errno=%d [%s]\n",errno,strerror(errno));
        exit(-1);
#else
          exit(-1);
#endif
      }

      while (1){
        printf("input message type(end:0):");
        scanf("%d",&msg_type);
        if (msg_type == 0)
           break;
        printf("input message to be sent:");
        scanf ("%s",str);
        msgs.msgtype = msg_type;
        strcpy(msgs.msgtext, str);
        /* 发送消息队列 */
        ret_value = msgsnd(msqid,&msgs,sizeof(struct msgstru),IPC_NOWAIT);
        if ( ret_value < 0 ) {
           printf("msgsnd() write msg failed,errno=%d[%s]\n",errno,strerror(errno));
           exit(-1);
        }
      }
      //msgctl(msqid,IPC_RMID,0); //删除消息队列

#endif
    return 0;
}
