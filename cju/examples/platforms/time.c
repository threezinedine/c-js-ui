#include <cju/cju.h>

int main(void)
{
	CuDateTime datetime = cuTimestampToDateTime(cuGetCurrentTimestamp());

	char buffer[100];
	cuBufferedString(buffer,
					 sizeof(buffer),
					 "Current Date and Time: %04u-%02u-%02u %02u:%02u:%02u.%03u",
					 datetime.year,
					 datetime.month,
					 datetime.day,
					 datetime.hours,
					 datetime.minutes,
					 datetime.seconds,
					 datetime.milliseconds);
	cuPrintLnFormat("%s", buffer);
	return 0;
}