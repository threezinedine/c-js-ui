#include <cju/cju.h>

int main(void)
{
	CuLoggerConfig config = {
		.minLogLevel = CU_LOG_LEVEL_DEBUG,
	};
	cuLoggerInitialize(&config);

	CuHandler consoleHandler = {.initialize = CU_NULL, .log = cuConsoleLoggerLog, .shutdown = CU_NULL};
	cuLoggerAddHandler(&consoleHandler);

	CU_LOG_TRACE("This is a TRACE level log message.");
	CU_LOG_DEBUG("This is a DEBUG level log message.");
	CU_LOG_INFO("This is an INFO level log message.");
	CU_LOG_WARN("This is a WARN level log message.");
	CU_LOG_ERROR("This is an ERROR level log message.");
	CU_LOG_FATAL("This is a FATAL level log message.");

	cuLoggerShutdown();
	return 0;
}