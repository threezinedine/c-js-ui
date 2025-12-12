#include "cju/core/log/log.h"

const char* cuLogLevelToString(CuLogLevel level)
{
	switch (level)
	{
	case CU_LOG_LEVEL_TRACE:
		return "TRACE";
	case CU_LOG_LEVEL_DEBUG:
		return "DEBUG";
	case CU_LOG_LEVEL_INFO:
		return "INFO";
	case CU_LOG_LEVEL_WARN:
		return "WARN";
	case CU_LOG_LEVEL_ERROR:
		return "ERROR";
	case CU_LOG_LEVEL_FATAL:
		return "FATAL";
	default:
		CU_UNREACHABLE();
	}
}