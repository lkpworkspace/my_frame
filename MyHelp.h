#ifndef __MyHelp_H__
#define __MyHelp_H__
#include <chrono>
#include <string>
#include <vector>
class MyHelp
{
public:
	static int ToInt(const char *str);
	static float ToFloat(const char *str);
	static double ToDouble(const char* str);
	static long double ToLDouble(const char* str);
	static long ToLong(const char* str, int base);
	static long long ToLLong(const char* str, int base);
	static unsigned long ToULong(const char* str, int base);
	static unsigned long long ToULLong(const char* str, int base);

	static std::string ToStr(int num, int base);
	static std::string ToStr(long num, int base);
	static std::string ToStr(unsigned long num, int base);

	static std::vector<std::string> SplitStr(std::string str, char split);

	static std::string GetCurTime();
	static std::time_t GetTimeStamp();

	static uint8_t CheckSum(uint8_t* buf, int len);
	// min : include; max : exclude
    static uint8_t RandomNum(int min, int max);

	static std::string ToMutlStr(uint8_t* buf, int len);
};
#endif
