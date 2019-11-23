#include "MainSystem.h"
#include <time.h>
#include <string>

using namespace std;

string GetTimeInfomation() {
	time_t timer;
	timer = time(NULL);
	struct tm local;
	localtime_s(&local, &timer);

	int year, month, day, hour, minute, second;
	year = local.tm_year + 1900;
	month = local.tm_mon + 1;
	day = local.tm_mday;
	hour = local.tm_hour;
	minute = local.tm_min;
	second = local.tm_sec;

	string ret = to_string(year) + to_string(month) + to_string(day) + "_" + to_string(hour) + "." + to_string(minute) + "." + to_string(second);
	return ret;
}