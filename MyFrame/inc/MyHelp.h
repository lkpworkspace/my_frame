#ifndef __MyHelp_H__
#define __MyHelp_H__
#include <chrono>
#include <string>
#include <vector>
class MyHelp
{
public:
    // convert string to num
    static int ToInt(const char *str);
	static float ToFloat(const char *str);
	static double ToDouble(const char* str);
	static long double ToLDouble(const char* str);
	static long ToLong(const char* str, int base);
	static long long ToLLong(const char* str, int base);
	static unsigned long ToULong(const char* str, int base);
	static unsigned long long ToULLong(const char* str, int base);

    // convert num to string
	static std::string ToStr(int num, int base);
	static std::string ToStr(long num, int base);
	static std::string ToStr(unsigned long num, int base);
    static char ToChar(uint8_t num);

    // string process
	static std::vector<std::string> SplitStr(std::string str, char split);
    static std::string ToMutlStr(uint8_t* buf, int len);

    // time
    static std::string GetCurTime();
	static std::time_t GetTimeStamp();

    // random function
    static uint8_t RandomNum(int min, int max); /* min : include; max : exclude */
    static std::string RandomString(int len);

    static uint8_t CheckSum(uint8_t* buf, int len);
};
#endif
