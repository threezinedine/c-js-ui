#define _POSIX_C_SOURCE 199309L
#include "cju/platform/platform.h"
#include <time.h>

CuTimestamp cuGetCurrentTimestamp()
{
	struct timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);
	return (CuTimestamp)(ts.tv_sec * 1000000ULL + ts.tv_nsec / 1000ULL);
}

CuDateTime cuTimestampToDateTime(CuTimestamp timestamp)
{
	time_t	   rawTime = (time_t)(timestamp / 1000000ULL);
	struct tm* timeInfo;
	timeInfo = gmtime(&rawTime);
	CuDateTime dateTime;
	dateTime.year		  = (u32)(timeInfo->tm_year + 1900);
	dateTime.month		  = (u32)(timeInfo->tm_mon + 1);
	dateTime.day		  = (u32)(timeInfo->tm_mday);
	dateTime.hours		  = (u32)(timeInfo->tm_hour);
	dateTime.minutes	  = (u32)(timeInfo->tm_min);
	dateTime.seconds	  = (u32)(timeInfo->tm_sec);
	dateTime.milliseconds = (u32)((timestamp % 1000000ULL) / 1000ULL);
	return dateTime;
}