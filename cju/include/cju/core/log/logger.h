#pragma once

#include "cju/core/containers/array.h"
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

/**
 * @brief Represents a log record containing information about a logging event.
 */
typedef struct CuLoggerRecord
{
	CuLogLevel	level;	   ///< The severity level of the log message.
	CuTimestamp timestamp; ///< The timestamp when the log message was created.
	const char* message;   ///< The actual log message.
	const char* file;	   ///< The source file where the log message was generated.
	u32			line;	   ///< The line number in the source file where the log message was generated.
} CuLoggerRecord;

/**
 * @brief Function pointer type for initializing a custom log handler.
 */
typedef void (*PFN_CUHandlerInitalize)();

/**
 * @brief Function pointer type for handling log records.
 */
typedef void (*PFN_CUHandlerLog)(const CuLoggerRecord* record);

/**
 * @brief Function pointer type for shutting down a custom log handler.
 */
typedef void (*PFN_CUHandlerShutdown)();

typedef struct CuHandler
{
	PFN_CUHandlerInitalize initialize; ///< Can be null.
	PFN_CUHandlerLog	   log;		   ///< Must not be null.
	PFN_CUHandlerShutdown  shutdown;   ///< Can be null.
} CuHandler;

typedef struct CuLoggerConfig
{
	CuLogLevel minLogLevel; ///< Minimum log level to be recorded.
} CuLoggerConfig;

/**
 * @brief Initializes the logging system.
 */
void cuLoggerInitialize(const CuLoggerConfig* pConfig);

/**
 * @brief Adds a log handler to the logger.
 *
 * @param pHandler Pointer to the CuHandler to be added.
 */
void cuLoggerAddHandler(const CuHandler* pHandler);

/**
 * @brief Logs a message with the specified log level, file, and line number.
 *
 * @param message The log message to be recorded.
 * @param file    The source file where the log message is generated.
 * @param line    The line number in the source file.
 * @param level   The severity level of the log message.
 */
void cuLoggerLog(const char* message, const char* file, u32 line, CuLogLevel level);

/**
 * @brief Shuts down the logging system and releases associated resources.
 *
 * @param pLogger Pointer to the CuLogger instance to be shut down.
 */
void cuLoggerShutdown();

#define _CU_LOG_INTERNAL(level, message, ...)                                                                          \
	do                                                                                                                 \
	{                                                                                                                  \
		char buffer[CU_BUFFER_SIZE];                                                                                   \
		cuBufferedString(buffer, sizeof(buffer), message, ##__VA_ARGS__);                                              \
		cuLoggerLog(buffer, __FILE__, __LINE__, level);                                                                \
	} while (CU_FALSE)

#define CU_LOG_TRACE(message, ...) _CU_LOG_INTERNAL(CU_LOG_LEVEL_TRACE, message, ##__VA_ARGS__)
#define CU_LOG_DEBUG(message, ...) _CU_LOG_INTERNAL(CU_LOG_LEVEL_DEBUG, message, ##__VA_ARGS__)
#define CU_LOG_INFO(message, ...)  _CU_LOG_INTERNAL(CU_LOG_LEVEL_INFO, message, ##__VA_ARGS__)
#define CU_LOG_WARN(message, ...)  _CU_LOG_INTERNAL(CU_LOG_LEVEL_WARN, message, ##__VA_ARGS__)
#define CU_LOG_ERROR(message, ...) _CU_LOG_INTERNAL(CU_LOG_LEVEL_ERROR, message, ##__VA_ARGS__)
#define CU_LOG_FATAL(message, ...) _CU_LOG_INTERNAL(CU_LOG_LEVEL_FATAL, message, ##__VA_ARGS__)