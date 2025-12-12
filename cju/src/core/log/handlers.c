#include "cju/core/log/log.h"
#include "cju/platform/platform.h"

void cuConsoleLoggerLog(const CuLoggerRecord* pRecord)
{
	CU_ASSERT(pRecord != CU_NULL);

	const char* levelStr = cuLogLevelToString(pRecord->level);

	switch (pRecord->level)
	{
	case CU_LOG_LEVEL_FATAL:
		cuConsoleSetColor(CU_CONSOLE_COLOR_MAGENTA);
		break;
	case CU_LOG_LEVEL_ERROR:
		cuConsoleSetColor(CU_CONSOLE_COLOR_RED);
		break;
	case CU_LOG_LEVEL_WARN:
		cuConsoleSetColor(CU_CONSOLE_COLOR_YELLOW);
		break;
	case CU_LOG_LEVEL_INFO:
		cuConsoleSetColor(CU_CONSOLE_COLOR_GREEN);
		break;
	case CU_LOG_LEVEL_DEBUG:
		cuConsoleSetColor(CU_CONSOLE_COLOR_BLUE);
		break;
	case CU_LOG_LEVEL_TRACE:
		cuConsoleSetColor(CU_CONSOLE_COLOR_WHITE);
		break;

	default:
		CU_UNREACHABLE();
		break;
	}

	cuPrintLnFormat("[%7s] [%7s] %s (%s:%u)", "SIMUI", levelStr, pRecord->message, pRecord->file, pRecord->line);
}