#include "../inc/MyHelp.h"
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <random>


#define UNUSE(arg) (arg)=(arg)

int MyHelp::ToInt(const char *str)
{
	return atoi(str);
}
float MyHelp::ToFloat(const char *str)
{
    char* pEnd;
	return strtof(str,&pEnd);
}
double MyHelp::ToDouble(const char* str)
{
	char* pEnd;
	return strtod(str,&pEnd);
}
long double MyHelp::ToLDouble(const char* str)
{
	char* pEnd;
	return strtold(str, &pEnd);
}
long MyHelp::ToLong(const char* str, int base)
{
	char* pEnd;
	return strtol(str, &pEnd, base);
}
long long MyHelp::ToLLong(const char* str, int base)
{
	char* pEnd;
	return strtoll(str, &pEnd, base);
}
unsigned long MyHelp::ToULong(const char* str, int base)
{
	char* pEnd;
	return strtoul(str, &pEnd, base);
}
unsigned long long MyHelp::ToULLong(const char* str, int base)
{
	char* pEnd;
	return strtoull(str, &pEnd, base);
}
std::string MyHelp::ToStr(int num, int base)
{
    UNUSE(base);
	char buf[20] = { 0 };
#ifdef WIN32
    itoa(num, buf, base);
#else
    sprintf(buf,"%d",num);
#endif
	return std::string(buf);
}
std::string MyHelp::ToStr(long num, int base)
{
    UNUSE(base);
	char buf[20] = { 0 };
#ifdef WIN32
    ltoa(num, buf, base);
#else
    sprintf(buf,"%ld",num);
#endif
	return std::string(buf);
}
std::string MyHelp::ToStr(unsigned long num, int base)
{
    UNUSE(base);
	char buf[20] = { 0 };
#ifdef WIN32
	ultoa(num, buf, base);
#else
    sprintf(buf,"%u",num);
#endif
	return std::string(buf);
}

char MyHelp::ToChar(uint8_t num)
{
    if(((char)num) < 0 || num > 10)
        return '\0';
    num += 48;
    return (char)num;
}

//struct tm *localtime(const time_t * calptr);
//  将日历时间变换成本地时间，考虑到本地时区和夏令时标志。
//size_t strftime(char *buf,size_t maxsize,const char *format,const struct tm *tmptr);
//  格式化时间输出
//  %a 星期几的简写
//  %A 星期几的全称
//  %b 月分的简写
//  %B 月份的全称
//  %c 标准的日期的时间串
//  %C 年份的后两位数字
//  %d 十进制表示的每月的第几天
//  %D 月/天/年
//  %e 在两字符域中，十进制表示的每月的第几天
//  %F 年-月-日
//  %g 年份的后两位数字，使用基于周的年
//  %G 年分，使用基于周的年
//  %h 简写的月份名
//  %H 24小时制的小时
//  %I 12小时制的小时
//  %j 十进制表示的每年的第几天
//  %m 十进制表示的月份
//  %M 十时制表示的分钟数
//  %n 新行符
//  %p 本地的AM或PM的等价显示
//  %r 12小时的时间
//  %R 显示小时和分钟：hh:mm
//  %S 十进制的秒数
//  %t 水平制表符
//  %T 显示时分秒：hh:mm:ss
//  %u 每周的第几天，星期一为第一天 （值从0到6，星期一为0）
//  %U 第年的第几周，把星期日做为第一天（值从0到53）
//  %V 每年的第几周，使用基于周的年
//  %w 十进制表示的星期几（值从0到6，星期天为0）
//  %x 标准的日期串
//  %X 标准的时间串
//  %y 不带世纪的十进制年份（值从0到99）
//  %Y 带世纪部分的十进制年份
//  %z，%Z 时区名称，如果不能得到时区名称则返回空字符。
//  %% 百分号
std::string MyHelp::GetCurTime()
{
	time_t now_time;
	char tmp[64] = { 0 };

	now_time = time(NULL);
    // 格式化时间输出
	strftime(tmp, sizeof(tmp), "%Y/%m/%d %X", localtime(&now_time));
	return std::string(tmp);
}

std::time_t MyHelp::GetTimeStamp()
{
	std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds> tp = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
	auto tmp = std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch());
	return tmp.count();
}

uint8_t MyHelp::CheckSum(uint8_t* buf, int len)
{
	uint8_t checksum = 0x00;
	for (int i = 0; i < len - 1; ++i)
	{
		checksum += buf[i];
	}
	return checksum;
}

// min : include; max : exclude
uint8_t MyHelp::RandomNum(int min, int max)
{
    static bool is_invoke = false;
    if(!is_invoke)
    {
        srand((unsigned int)time(NULL));
        is_invoke = true;
    }
	uint8_t temp = 0x00;

	if (max < min) return 0x00;
	temp = rand() % (max - min) + min;
	return temp;
}

std::string MyHelp::RandomString(int len)
{
    char ch;
    std::string res;
    res.clear();
    uint8_t en_tab[] = {
        'A','B','C','D','E','F','G','H',
        'I','J','K','L','M','N','O','P',
        'Q','R','S','T','U','V','W','X',
        'Y','Z','a','b','c','d','e','f',
        'g','h','i','j','k','l','m','n',
        'o','p','q','r','s','t','u','v',
        'w','x','y','z','0','1','2','3',
        '4','5','6','7','8','9'
    };
    for(int i = 0; i < len; ++i)
    {
        int index = RandomNum(0,sizeof(en_tab));
        ch = en_tab[index];
        res += ch;
    }
    return res;
}

std::string MyHelp::ToMutlStr(uint8_t* buf, int len)
{
	std::string temp;
	uint8_t *temp_buf1 = new uint8_t[len * 2];
	char temp_buf2[4] = { 0 };
	int j = 0;

	temp += "  ";
	memset(temp_buf1, 0, len * 2);
	for (int i = 0; i < len * 2; i += 2)
	{
		temp_buf1[i] = buf[j++];
		sprintf(temp_buf2, "%x", temp_buf1[i]);
		if (temp_buf1[i] <= 0x0f)
		{
			temp += '0';
			temp += temp_buf2;
		}
		else
		{
			temp += temp_buf2;
		}
		temp += " ";
		memset(temp_buf2, 0, 4);
	}
	delete temp_buf1;
	return temp;
}

std::vector<std::string> MyHelp::SplitStr(std::string str, char split)
{
    int pos;
    std::vector<std::string> res;
    str += split;
    int size = str.size();

    for (int i = 0; i < size; ++i)
    {
        pos = str.find(split, i);
        if (pos < size)
        {
            std::string s = str.substr(i, pos - i);
            res.push_back(s);
            i = pos;
        }
    }
    return res;
}

/*
#include <dlfcn.h>
函数定义：　void * dlopen( const char * pathname, int mode);
函数描述：　在dlopen（）函数以指定模式打开指定的动态连接库文件，并返回一个句柄给调用进程。使用dlclose（）来卸载打开的库。
    mode：分为这两种RTLD_LAZY 暂缓决定，等有需要时再解出符号;
    RTLD_NOW 立即决定，返回前解除所有未决定的符号。
    RTLD_LOCAL 与RTLD_GLOBAL作用相反，动态库中定义的符号不能被其后打开的其它库重定位。
    如果没有指明是RTLD_GLOBAL还是RTLD_LOCAL，则缺省为RTLD_LOCAL。
    RTLD_GLOBAL 动态库中定义的符号可被其后打开的其它库重定位。
    RTLD_GROUPRTLD_WORLD
返回值:　打开错误返回NULL，成功，返回库引用。
编译时候要加入 -ldl (指定dl库)：例如　gcc test.c -o test -ldl

如果一个函数库里面有一个输出的函数名字为_init,那么_init就会在dlopen（）这个函数返回前被执行。
我们可以利用这个函数在我的函数库里面做一些初始化的工作。我们后面会继续讨论这个问题的。
*/
void* MyHelp::DllOpen(const char * pathname, int mode)
{
    // RTLD_NOW(use this)
    // RTLD_LAZY
    //
    void* handle;
    void* error;

    handle = dlopen(pathname,mode);
    error = dlerror();
    if(error)
    {
        printf("Open Dll %s failed, %s\n",pathname, (char*)error);
        return NULL;
    }
    return handle;
}

void* MyHelp::DllUse(void *handle, const char *symbol)
{
    void* func;
    void* error;

    func = dlsym(handle, symbol);
    error = dlerror();
    if(error)
    {
        printf("Use DLL sym failed, %s\n", (char*)error);
        return NULL;
    }
    return func;
}

int MyHelp::DllClose(void *handle)
{
    int res;
    void* error;
    res = dlclose(handle);
    if(res != 0)
    {
        error = dlerror();
        if(error)
        {
            printf("DLL close failed, %s\n", (char*)error);
            return -1;
        }
    }
    return res;
}

const char MyHelp::getch()
{
        char buf = 0;
        struct termios old = {0};
        if (tcgetattr(0, &old) < 0)
                perror("tcsetattr()");
        old.c_lflag &= ~ICANON;
        old.c_lflag &= ~ECHO;
        old.c_cc[VMIN] = 1;
        old.c_cc[VTIME] = 0;
        if (tcsetattr(0, TCSANOW, &old) < 0)
                perror("tcsetattr ICANON");
        if (read(0, &buf, 1) < 0)
                perror ("read()");
        old.c_lflag |= ICANON;
        old.c_lflag |= ECHO;
        if (tcsetattr(0, TCSADRAIN, &old) < 0)
                perror ("tcsetattr ~ICANON");
        return (buf);
}

bool MyHelp::SetNonblock(int fd, bool b)
{
    int flags = fcntl(fd, F_GETFL, 0);
    if(b)
        flags |= O_NONBLOCK;
    else
        flags &= ~O_NONBLOCK;
    return fcntl(fd, F_SETFL, flags) != -1;
}

int MyHelp::GetFileLen(const char* path)
{
    struct stat st;
    int ret = lstat(path, &st);
    if(ret < 0)
        return ret;

    if(S_ISREG(st.st_mode))
        return st.st_size;
    return 0;
}

bool MyHelp::IsFileExist(const char* path)
{
    struct stat st;
    int ret = lstat(path, &st);
    if(ret < 0)
        return false;

    if(S_ISREG(st.st_mode))
        return true;
    else
        return false;
}

bool MyHelp::IsDirExist(const char* path)
{
    struct stat st;
    int ret = lstat(path, &st);
    if(ret < 0)
        return 0;

    if(S_ISDIR(st.st_mode))
        return true;
    else
        return false;
}

char* MyHelp::GetHomeDir()
{
    return getenv("HOME");
}

uint64_t MyHelp::GetTimerNow()
{
    uint64_t now;

    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);

    now = ts.tv_sec * 1000u;
    now += ts.tv_nsec / 1000000u;
    return now;
}

int MyHelp::BytesAvailable(int fd)
{
    int len = 0, res = -1;
    res = ioctl(fd,FIONREAD,&len);
    return len;
}

void MyHelp::DaemonInit()
{
    pid_t pid;
    if((pid = fork()) < 0)
    {
            perror("fork");
            exit(-1);
    }else if(pid != 0)
        exit(0);
    setsid();

//    if(chdir("/") < 0)
//    {
//        perror("chdir");
//        exit(-1);
//    }
    umask(0);
    close(0);
    open("/dev/null",O_RDWR);
    dup2(0,1);
    dup2(0,2);
}

/// MyHelp end
/////////////////////////////////////////////////////////////////////////////////////////





/////////////////////////////////////////////////////////////////////////////////////////
/// MySelfProtocol
char *MySelfProtocol::GetBuf(int *len)
{
    if(len != NULL)
        *len = MYPROTO_MAX_BUF_SIZE;
    return (char*)malloc(MYPROTO_MAX_BUF_SIZE);
}

void MySelfProtocol::FreeBuf(char* buf)
{
    if(buf != NULL)
        free(buf);
}

////////////////////////////////////////////////
/// handle
uint16_t MySelfProtocol::HandleHeader(const char *buf)
{
    uint16_t head;
    memcpy(&head,&buf[0],sizeof(uint16_t));
    return head;
}

uint16_t MySelfProtocol::HandleShort(int offset, const char* buf, int len)
{
    if(offset + sizeof(uint16_t) > len)
        return 0;
    uint16_t data_len = 0;
    memcpy(&data_len,&buf[offset],sizeof(data_len));
    return data_len;
}

uint8_t MySelfProtocol::HandleChar(int offset, const char *buf, int len)
{
    if(offset + sizeof(uint8_t) > len)
        return 0;
    uint8_t data_len = 0;
    memcpy(&data_len,&buf[offset],sizeof(uint8_t));
    return data_len;
}

std::string MySelfProtocol::HandleString(int offset, const char *buf, int len)
{
    char temp_buf[MYPROTO_MAX_BUF_SIZE] = {0};
    int str_len = strlen(&buf[offset]);
    memcpy(temp_buf,&buf[offset],str_len);
    return std::string(temp_buf);
}

data_t MySelfProtocol::HandleData(int offset, const char* buf, int len)
{
    data_t my_data;

    my_data.len = HandleShort(offset,buf,len);
    memcpy(my_data.buf,&buf[offset + sizeof(uint16_t)],my_data.len);
    return my_data;
}

////////////////////////////////////////////////
/// build
int MySelfProtocol::BuildHeader(uint16_t head, char* buf, int len)
{
    memset(buf,0,len);
    memcpy(buf,&head,sizeof(head));
    return sizeof(head);
}

int MySelfProtocol::BuildShort(uint16_t datalen, int offset, char* buf, int len)
{
    if(offset + sizeof(datalen) > len)
        return 0;
    memcpy(&buf[offset],&datalen,sizeof(datalen));
    return sizeof(datalen);
}

int MySelfProtocol::BuildChar(uint8_t ch,int offset, char* buf, int len)
{
    if(offset + sizeof(ch) > len)
        return 0;
    memcpy(&buf[offset],&ch,sizeof(ch));
    return sizeof(ch);
}

int MySelfProtocol::BuildString(const char* str, int offset, char* buf, int len)
{
    int str_len = strlen(str);
    if(str_len + 1 + offset > len)
        return 0;
    memcpy(&buf[offset],str,str_len);
    return (str_len + 1);
}

int MySelfProtocol::BuildData(const char* data, uint16_t data_len, int offset, char* buf, int len)
{
    int len_offset = BuildShort(data_len,offset,buf,len);
    if(data_len + offset + len_offset > len)
        return 0;
    memcpy(&buf[offset + len_offset],data,data_len);
    return (data_len + len_offset);
}

/// MySelfProtocol end
/////////////////////////////////////////////////////////////////////////////////////////////////////



























