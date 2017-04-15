#include "MyTFTP.h"
#include "MyApp.h"
#include "MyLog.h"
using namespace my_master;
MyTFTP::MyTFTP(std::string ip,uint16_t port, bool isServer)
    :MyUdp(ip,port,isServer),
      m_path("")
{
    m_recv.block_num = 0;
    m_recv.fp = nullptr;
}

MyTFTP::~MyTFTP()
{}

void* MyTFTP::CallBackFunc(MyEvent *ev)
{
    MyTFTP* client = (MyTFTP*)ev;
    char* buf;
    int len;
#ifdef DEBUG_TFTP
    MyDebugPrint("MyTFTP get msg\n");
#endif
    MyAddrInfo info = client->RecvData(&buf,len);

    HandleMsg(info,buf,len);

    MyApp::theApp->AddEvent(ev);
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

    strcpy(filename,&buf[TFTP_HEAD_SIZE]);
    // send ack
    int send_len = BuildACK(0);
    this->Write(info,m_buf,send_len);
    // send file
    InitFileTrans(filename,info);
    this->Start();
    return 0;
}

int MyTFTP::HandleMsg2(MyAddrInfo& info,char* buf, int len)
{ // write request
    char filename[256] = {0};
    std::string fillPath;
    // init m_recv
    m_recv.block_num = 0;
    if(m_recv.fp != nullptr)
        fclose(m_recv.fp);
    // open file, send ack
    strcpy(filename,&buf[TFTP_HEAD_SIZE]);
    fillPath += m_path;
    fillPath += filename;
    m_recv.fp = fopen(fillPath.c_str(),"w");
    int send_len = BuildACK(0);
#ifdef DEBUG_TFTP
    MyDebugPrint("get w file request\t");
    MyDebugPrint("filename %s\n",filename);
#endif
    this->Write(info,m_buf,send_len);
    return 0;
}

int MyTFTP::HandleMsg3(MyAddrInfo& info,char* buf, int len)
{ // get data, only ascii
    int b_num;
    int data_len;

    memcpy(&b_num,&buf[TFTP_HEAD_SIZE],sizeof(uint16_t));
#ifdef DEBUG_TFTP
    MyDebugPrint("buf len %d, recv_num %d, cur_block_num %d\n",len,b_num, m_recv.block_num);
#endif
    if(b_num - m_recv.block_num == 1)
    {
        data_len = strlen(&buf[TFTP_HEAD_SIZE + TFTP_BLOCKNUM_SIZE]);
        fwrite(&buf[4],data_len,1,m_recv.fp);
        int sendLen = BuildACK(b_num);
        // ack
        Write(info,m_buf,sendLen);
#ifdef DEBUG_TFTP
    MyDebugPrint("get %d block, data len %d\r", m_recv.block_num, data_len);
#endif
        m_recv.block_num++;
    }else
    { // send err msg
#ifdef DEBUG_TFTP
    MyDebugPrint("recv file erro\n");
#endif
        int sendLen = BuildErrMsg(0x01,"err block num");
        // ack
        Write(info,m_buf,sendLen);
    }
    if(data_len != TFTP_DATA_SIZE)
    { // recv data end
#ifdef DEBUG_TFTP
    MyDebugPrint("recv file end... data len %d\n",data_len);
#endif
        fclose(m_recv.fp);
        memset(&m_recv,0,sizeof(recv_t));
    }
    return (TFTP_HEAD_SIZE + TFTP_BLOCKNUM_SIZE + TFTP_DATA_SIZE + 1);
}

int MyTFTP::HandleMsg4(MyAddrInfo& info,char* buf, int len)
{ // recv ack
    int block_num = GetBlockNum(buf);
    switch (block_num) {
    case 0x00:
        sem_post(&m_send.event);
        break;
    default:
        if(block_num - m_send.block_num == 0)
        {
            sem_post(&m_send.event);
        }else
        {
#ifdef DEBUG_TFTP
            printf("Send file fail...\n");
#endif
            this->Stop();
            sem_post(&m_send.event);
        }
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
    memcpy(&m_buf[TFTP_HEAD_SIZE],&block_num,sizeof(uint16_t));
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
void MyTFTP::OnInit()
{
    usleep(1000*100);
    // read file data
    ReadFile(m_send.filename);
    // send file info
    int sendLen = BuildWRQ(m_send.filename);
#ifdef DEBUG_TFTP
    MyDebugPrint("send file info send_len %d, filename %s\n",sendLen,m_send.filename.c_str());
#endif
    this->Write(m_send.info,m_buf,sendLen);
    // wait ack
    sem_wait(&m_send.event);
#ifdef DEBUG_TFTP
    MyDebugPrint("get file info ack\n");
#endif
}

void MyTFTP::Run()
{ // send file data and wait ack
    char* buf;
    int data_len = GetFileData(&buf);
    if(data_len > 0)
    {
        m_send.block_num++;
#ifdef DEBUG_TFTP
    MyDebugPrint("file block num %d\n",m_send.block_num);
#endif
        int send_len = BuildData(m_send.block_num,buf,data_len);
#ifdef DEBUG_TFTP
    MyDebugPrint("send file data, data_len %d\n",send_len);
#endif
#ifdef DEBUG_TFTP
    for(int i = 0; i < 10; ++i)
    {
        printf("%02x\t",m_buf[i]);
    }
#endif
        // send data
        this->Write(m_send.info,m_buf,send_len);
        sem_wait(&m_send.event);
    }
    else
    {
        this->Stop();
    }
}

void MyTFTP::OnExit()
{
    CloseFileTrans();
}

void MyTFTP::GetFile(MyAddrInfo& info, std::string filename)
{ // TODO...
    int send_len = BuildRRQ(filename);
    this->Write(info,m_buf,send_len);
}

void MyTFTP::SendFile(MyAddrInfo& info, std::string filename)
{ // TODO...
    InitFileTrans(filename,info);

    this->Start();
}

int MyTFTP::GetFileData(char** buf)
{ // if return < 0, end of file
    *buf = &m_send.file_buf[m_send.send_block * TFTP_DATA_SIZE];
    int res = (m_send.send_block + 1) * TFTP_DATA_SIZE > m_send.file_len
            ? m_send.file_len - m_send.send_block * TFTP_DATA_SIZE
              : TFTP_DATA_SIZE;
    m_send.send_block++;
    return res;
}

void MyTFTP::ReadFile(std::string filename)
{
    std::string fill_path = m_path + filename;
    m_send.file_len = Common::GetFileLen(fill_path.c_str());
    m_send.file_buf = (char*)malloc(m_send.file_len);
    fread(m_send.file_buf,m_send.file_len,1,m_send.fp);
}

void MyTFTP::InitFileTrans(std::string filename, MyAddrInfo& info)
{
    if(m_send.fp != nullptr)
        fclose(m_send.fp);
    std::string fill_path = m_path + filename;
    m_send.fp = fopen(fill_path.c_str(),"r");
    sem_init(&m_send.event,0,0);
    m_send.filename = filename;
    m_send.info = info;
    m_send.block_num = 0;
    m_send.send_block = 0;
    if(m_send.file_buf != nullptr)
        free(m_send.file_buf);
    m_send.file_buf = nullptr;
    m_send.file_len = 0;
}

void MyTFTP::CloseFileTrans()
{
    if(m_send.fp != nullptr)
        fclose(m_send.fp);
    m_send.fp = nullptr;
    m_send.filename.clear();
    m_send.block_num = 0;
    m_send.send_block = 0;
    if(m_send.file_buf != nullptr)
        free(m_send.file_buf);
    m_send.file_buf = nullptr;
    m_send.file_len = 0;
    sem_destroy(&m_send.event);
}
























