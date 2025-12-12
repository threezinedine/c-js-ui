#pragma once

#include "cju/platform/platform.h"

typedef enum CuLogLevel
{
	CU_LOG_LEVEL_TRACE, ///< Detailed information, typically of interest only when diagnosing problems.
	CU_LOG_LEVEL_DEBUG, ///< Information useful to developers for debugging the application.
	CU_LOG_LEVEL_INFO,	///< General operational information about the application.
	CU_LOG_LEVEL_WARN,	///< Indication of potential issues or important events that are not errors.
	CU_LOG_LEVEL_ERROR, ///< Errors that might still allow the application to continue running.
	CU_LOG_LEVEL_FATAL, ///< Severe errors that will lead the application to abort.
} CuLogLevel;

typedef struct CuLoggerRecord
{
	CuLogLevel	level;
	const char* message;
	const char* file;
	u32			line;
} CuLoggerRecord;

typedef struct CuLogger
{
} CuLogger;