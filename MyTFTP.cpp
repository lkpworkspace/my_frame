#include "MyTFTP.h"
using namespace my_master;
MyTFTP::MyTFTP(std::string ip,uint16_t port, bool isServer)
    :MyUdp(ip,port,isServer),
      m_path("")
{
    m_recv.block_num = 0;
    m_recv.fp = nullptr;

    m_send.fp = nullptr;
}

MyTFTP::~MyTFTP()
{

}

void MyTFTP::SendFile(std::string filename)
{
    m_send.m_send_filename = filename;
    this->Start();
}

void* MyTFTP::CallBackFunc(MyEvent *ev)
{
    MyTFTP* client = (MyTFTP*)ev;
    char* buf;
    int len;

    MyAddrInfo info = client->RecvData(&buf,len);
    HandleMsg(info,buf,len);
    return NULL;
}

int MyTFTP::HandleMsg(MyAddrInfo& info, char* buf, int len)
{
    switch(GetHead(buf))
    {
    case 0x01: // read request
        HandleMsg1(info,buf,len);
        break;
    case 0x02: // write request
        HandleMsg2(info,buf,len);
        break;
    case 0x03: // get data
        HandleMsg3(info,buf,len);
        break;
    case 0x04: // ack
        HandleMsg4(info,buf,len);
        break;
    case 0x05: // err
        HandleMsg5(info,buf,len);
        break;
    default:
        break;
    }
    return 0;
}


int MyTFTP::HandleMsg1(MyAddrInfo& info,char* buf, int len)
{ // read request
    char filename[256] = {0};
    std::string fillPath;

    strcpy(filename,&buf[TFTP_HEAD_SIZE]);
    fillPath += m_path;
    fillPath += filename;
    // begin thread to send file
    // ...

    // open file
    m_send.fp = fopen(fillPath.c_str(),"r");
    int sendLen = BuildACK(0);
    this->Write(info,m_buf,sendLen);
    return 0;
}

int MyTFTP::HandleMsg2(MyAddrInfo& info,char* buf, int len)
{ // write request
    char filename[256] = {0};
    std::string fillPath;

    strcpy(filename,&buf[TFTP_HEAD_SIZE]);
    fillPath += m_path;
    fillPath += filename;
    m_recv.fp = fopen(fillPath.c_str(),"w");
    int sendLen = BuildACK(0);
    this->Write(info,m_buf,sendLen);
    // init m_recv
    m_recv.block_num = 0;
    if(m_recv.fp != nullptr)
        fclose(m_recv.fp);
    return 0;
}

int MyTFTP::HandleMsg3(MyAddrInfo& info,char* buf, int len)
{ // get data, only ascii
    int b_num;
    int data_len;

    memcpy(&b_num,&buf[TFTP_HEAD_SIZE],sizeof(uint16_t));
    if(b_num - m_recv.block_num == 1)
    {
        data_len = strlen(&buf[TFTP_HEAD_SIZE + TFTP_BLOCKNUM_SIZE]);
        fwrite(&buf[4],data_len,1,m_recv.fp);
        int sendLen = BuildACK(b_num);
        Write(info,m_buf,sendLen);

        m_recv.block_num++;
    }else
    { // send err msg
        int sendLen = BuildErrMsg(0x01,"err block num");
        Write(info,m_buf,sendLen);
    }
    if(data_len != TFTP_DATA_SIZE)
    { // recv data end
        fclose(m_recv.fp);
        memset(&m_recv,0,sizeof(recv_t));
    }
    return (TFTP_HEAD_SIZE + TFTP_BLOCKNUM_SIZE + TFTP_DATA_SIZE + 1);
}

int MyTFTP::HandleMsg4(MyAddrInfo& info,char* buf, int len)
{
    switch (GetBlockNum(buf)) {
    case 0x00:
        // TODO...
        break;
    default:

        break;
    }
    return 0;
}

int MyTFTP::HandleMsg5(MyAddrInfo& info,char* buf, int len)
{
    return 0;
}

uint16_t MyTFTP::GetHead(char* buf)
{
    uint16_t head;
    memcpy(&head,&buf[0],sizeof(uint16_t));
    return head;
}

uint16_t MyTFTP::GetBlockNum(char* buf)
{
    uint16_t block_num;
    memcpy(&block_num,&buf[2],sizeof(uint16_t));
    return block_num;
}

int MyTFTP::BuildRRQ(std::string filename)
{
    uint16_t num = 0x01;
    memset(m_buf,0,TFTP_BUF_SIZE);
    memcpy(&m_buf[0],&num,sizeof(uint16_t));
    memcpy(&m_buf[TFTP_HEAD_SIZE],filename.c_str(),filename.size());
    return (TFTP_HEAD_SIZE + filename.size() + 1);
}

int MyTFTP::BuildWRQ(std::string filename)
{
    uint16_t num = 0x02;
    memset(m_buf,0,TFTP_BUF_SIZE);
    memcpy(&m_buf[0],&num,sizeof(uint16_t));
    memcpy(&m_buf[TFTP_HEAD_SIZE],filename.c_str(),filename.size());
    return (TFTP_HEAD_SIZE + filename.size() + 1);
}

int MyTFTP::BuildData(uint16_t block_num,char* buf, uint16_t len) // len (0-512byte)
{
    uint16_t num = 0x03;
    memset(m_buf,0,TFTP_BUF_SIZE);
    memcpy(&m_buf[0],&num,sizeof(uint16_t));
    memcpy(&buf[TFTP_HEAD_SIZE],&block_num,sizeof(uint16_t));
    memcpy(&m_buf[TFTP_HEAD_SIZE + TFTP_BLOCKNUM_SIZE],buf,len);
    return (TFTP_HEAD_SIZE + TFTP_BLOCKNUM_SIZE + len + 1);
}

int MyTFTP::BuildACK(uint16_t block_num)
{
    uint16_t num = 0x04;
    memset(m_buf,0,TFTP_BUF_SIZE);
    memcpy(&m_buf[0],&num,sizeof(uint16_t));
    memcpy(&m_buf[TFTP_HEAD_SIZE],&block_num,sizeof(uint16_t));
    return (TFTP_HEAD_SIZE + TFTP_BLOCKNUM_SIZE + 1);
}

int MyTFTP::BuildErrMsg(uint16_t err_num, std::string errMsg)
{
    uint16_t num = 0x05;
    memset(m_buf,0,TFTP_BUF_SIZE);
    memcpy(&m_buf[0],&num,sizeof(uint16_t));
    memcpy(&m_buf[TFTP_HEAD_SIZE],&err_num,sizeof(uint16_t));
    memcpy(&m_buf[TFTP_HEAD_SIZE + TFTP_ERRNUM_SIZE],errMsg.c_str(),errMsg.size());
    return (TFTP_HEAD_SIZE + TFTP_ERRNUM_SIZE + errMsg.size() + 1);
}

void MyTFTP::SetRootDir(std::string path)
{
    m_path = path;
}

/////////////////////////////////////////////////////////
void MyTFTP::Run()
{ // loop

}

void MyTFTP::OnInit()
{
    usleep(1000*100);
}

void MyTFTP::OnExit()
{

}
