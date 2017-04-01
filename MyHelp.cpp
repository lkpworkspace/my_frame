#include "MyHelp.h"

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
	char buf[20] = { 0 };
#ifdef WIN32
	ultoa(num, buf, base);
#else
    sprintf(buf,"%u",num);
#endif
	return std::string(buf);
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

std::string MyHelp::GetCurTime()
{
	time_t now_time;
	char tmp[64] = { 0 };

	now_time = time(NULL);
	strftime(tmp, sizeof(tmp), "%Y/%m/%d %X", localtime(&now_time));
	return std::string(tmp);
}
//struct tm *localtime(const time_t *timer)·µ»ØÒ»¸öÒÔtm½á¹¹±í´ïµÄ»úÆ÷Ê±¼äÐÅÏ¢
//char* asctime(struct tm * ptr)µÃµ½»úÆ÷Ê±¼ä(ÈÕÆÚÊ±¼ä×ª»»ÎªASCIIÂë)
//char *ctime(const time_t *time)µÃµ½ÈÕÀúÊ±¼ä
//double difftime(time_t time2, time_t time1)µÃµ½Á½´Î»úÆ÷Ê±¼ä²î£¬µ¥Î»ÎªÃë
//struct tm *gmtime(time_t *time)µÃµ½ÒÔ½á¹¹tm±íÊ¾µÄÊ±¼äÐÅÏ¢
//size_t strftime(char *strDest, size_t maxsize, const char *format, const struct tm *timeptr);
//¸ù¾Ý¸ñÊ½×Ö·û´®Éú³É×Ö·û´®¡£
//struct tm *localtime(const time_t *timer);
//È¡µÃµ±µØÊ±¼ä£¬localtime»ñÈ¡µÄ½á¹ûÓÉ½á¹¹tm·µ»Ø
//·µ»ØµÄ×Ö·û´®¿ÉÒÔÒÀÏÂÁÐµÄ¸ñÊ½¶ø¶¨£º
//%a ÐÇÆÚ¼¸µÄËõÐ´¡£Eg : Tue
//%A ÐÇÆÚ¼¸µÄÈ«Ãû¡£ Eg : Tuesday
//%b ÔÂ·ÝÃû³ÆµÄËõÐ´¡£
//%B ÔÂ·ÝÃû³ÆµÄÈ«Ãû¡£
//%c ±¾µØ¶ËÈÕÆÚÊ±¼ä½Ï¼Ñ±íÊ¾×Ö·û´®¡£
//%d ÓÃÊý×Ö±íÊ¾±¾ÔÂµÄµÚ¼¸Ìì(·¶Î§Îª 00 ÖÁ 31)¡£ÈÕÆÚ
//%H ÓÃ 24 Ð¡Ê±ÖÆÊý×Ö±íÊ¾Ð¡Ê±Êý(·¶Î§Îª 00 ÖÁ 23)¡£
//%I ÓÃ 12 Ð¡Ê±ÖÆÊý×Ö±íÊ¾Ð¡Ê±Êý(·¶Î§Îª 01 ÖÁ 12)¡£
//%j ÒÔÊý×Ö±íÊ¾µ±Äê¶ÈµÄµÚ¼¸Ìì(·¶Î§Îª 001 ÖÁ 366)¡£
//%m ÔÂ·ÝµÄÊý×Ö(·¶Î§ÓÉ 1 ÖÁ 12)¡£
//%M ·ÖÖÓ¡£
//%p ÒÔ ''AM'' »ò ''PM'' ±íÊ¾±¾µØ¶ËÊ±¼ä¡£
//%S ÃëÊý¡£
//%U Êý×Ö±íÊ¾Îª±¾Äê¶ÈµÄµÚ¼¸ÖÜ£¬µÚÒ»¸öÐÇÆÚÓÉµÚÒ»¸öÖÜÈÕ¿ªÊ¼¡£
//%W Êý×Ö±íÊ¾Îª±¾Äê¶ÈµÄµÚ¼¸ÖÜ£¬µÚÒ»¸öÐÇÆÚÓÉµÚÒ»¸öÖÜÒ»¿ªÊ¼¡£
//%w ÓÃÊý×Ö±íÊ¾±¾ÖÜµÄµÚ¼¸Ìì(0 ÎªÖÜÈÕ)¡£
//%x ²»º¬Ê±¼äµÄÈÕÆÚ±íÊ¾·¨¡£
//%X ²»º¬ÈÕÆÚµÄÊ±¼ä±íÊ¾·¨¡£ Eg : 15 : 26 : 30
//% y ¶þÎ»Êý×Ö±íÊ¾Äê·Ý(·¶Î§ÓÉ 00 ÖÁ 99)¡£
//%Y ÍêÕûµÄÄê·ÝÊý×Ö±íÊ¾£¬¼´ËÄÎ»Êý¡£ Eg : 2008
//% Z(%z) Ê±Çø»òÃû³ÆËõÐ´¡£Eg : ÖÐ¹ú±ê×¼Ê±¼ä
//%% % ×Ö·û¡£
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
uint8_t MyHelp::RandomNum(int min, int max) // Éú³ÉÒ»×Ö½ÚµÄËæ»úÊý
{
	uint8_t temp = 0x00;

	if (max < min) return 0x00;
	temp = rand() % (max - min) + min;
	return temp;
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

