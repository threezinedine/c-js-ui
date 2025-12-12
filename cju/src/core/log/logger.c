#include "cju/core/log/log.h"
#include <string.h>

typedef struct CuLogger
{
	CuArray*   pHandlers;	///< Array of CuHandler
	CuLogLevel minLogLevel; ///< Minimum log level for this logger.
} CuLogger;

CuArray* s_pLoggers = CU_NULL;

void cuLoggerInitialize(const CuLoggerConfig* pConfig)
{
	CU_ASSERT(s_pLoggers == CU_NULL);
	s_pLoggers = CU_ARRAY_INIT(CuLogger, 4);

	CuLogger rootLogger = {.pHandlers = CU_ARRAY_INIT(CuHandler, 4), .minLogLevel = pConfig->minLogLevel};
	CU_ARRAY_PUSH_BACK(CuLogger, s_pLoggers, rootLogger);
}

void cuLoggerAddHandler(const CuHandler* pHandler)
{
	CU_ASSERT(s_pLoggers != CU_NULL);
	CU_ASSERT(pHandler != CU_NULL);
	CU_ASSERT(pHandler->log != CU_NULL);

	CuLogger* pLogger = CU_ARRAY_GET(CuLogger, s_pLoggers, 0); // Get root logger for now

	CU_ASSERT(pLogger != CU_NULL);
	CU_ASSERT(pLogger->pHandlers != CU_NULL);

	CU_ARRAY_PUSH_BACK(CuHandler, pLogger->pHandlers, *pHandler);
}

void cuLoggerLog(const char* message, const char* file, u32 line, CuLogLevel level)
{
	CU_ASSERT(s_pLoggers != CU_NULL);

	CuLogger* pLogger = CU_ARRAY_GET(CuLogger, s_pLoggers, 0); // Get root logger for now

	if (level < pLogger->minLogLevel)
	{
		return;
	}

	CU_ASSERT(pLogger != CU_NULL);
	CU_ASSERT(pLogger->pHandlers != CU_NULL);

	CuLoggerRecord record = {
		.level	   = level,
		.timestamp = cuGetCurrentTimestamp(),
		.message   = message,
		.file	   = file,
		.line	   = line,
	};

	for (u32 i = 0; i < pLogger->pHandlers->count; ++i)
	{
		CuHandler* pHandler = CU_ARRAY_GET(CuHandler, pLogger->pHandlers, i);
		CU_ASSERT(pHandler != CU_NULL);
		CU_ASSERT(pHandler->log != CU_NULL);

		pHandler->log(&record);
	}
}

void cuLoggerShutdown(CuLogger* pLogger)
{
	CU_ASSERT(s_pLoggers != CU_NULL);
}