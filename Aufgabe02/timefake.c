#include <stdio.h>
#include <time.h>

struct tm *localtime(const time_t * foo)
{
	static struct tm test;

	test.tm_sec = 10;
	test.tm_min = 10;
	test.tm_hour = 10;
	test.tm_mday = 10;
	test.tm_mon = 10;
	test.tm_year = 1910;
	test.tm_wday = 1;
	test.tm_yday = 1;
	test.tm_isdst = 0;

	return &test;
}