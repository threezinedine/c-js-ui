#include "cju/platform/platform.h"
#include "stdarg.h"
#include "stdio.h"

void cuConsoleConfig(CuConsoleConfig config)
{
	cuConsoleSetColor(config.textColor);
}

void cuConsoleSetColor(CuConsoleColor color)
{
#if CU_PLATFORM_WINDOWS
#error "Windows platform is not yet supported"
#elif CU_PLATFORM_UNIX || CU_PLATFORM_WEB
	const char* colorCode;
	switch (color)
	{
	case CU_CONSOLE_COLOR_BLACK:
		colorCode = "\033[30m";
		break;
	case CU_CONSOLE_COLOR_RED:
		colorCode = "\033[31m";
		break;
	case CU_CONSOLE_COLOR_GREEN:
		colorCode = "\033[32m";
		break;
	case CU_CONSOLE_COLOR_YELLOW:
		colorCode = "\033[33m";
		break;
	case CU_CONSOLE_COLOR_BLUE:
		colorCode = "\033[34m";
		break;
	case CU_CONSOLE_COLOR_MAGENTA:
		colorCode = "\033[35m";
		break;
	case CU_CONSOLE_COLOR_CYAN:
		colorCode = "\033[36m";
		break;
	case CU_CONSOLE_COLOR_WHITE:
		colorCode = "\033[37m";
		break;
	case CU_CONSOLE_COLOR_RESET:
		colorCode = "\033[0m";
		break;
	case CU_CONSOLE_COLOR_DEFAULT:
	default:
		colorCode = "\033[39m";
		break;
	}
	printf("%s", colorCode);
#else
#error "Unsupported platform"
#endif
}

void cuConsolePrint(const char* message)
{
	printf("%s", message);
}

void cuConsolePrintLn(const char* message)
{
	printf("%s\n", message);
}

void cuConsolePrintFormat(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	vprintf(format, args);
	va_end(args);
}

void cuConsolePrintLnFormat(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	vprintf(format, args);
	printf("\n");
	va_end(args);
}

void cuBufferedString(char* buffer, u32 bufferSize, const char* format, ...)
{
	va_list args;
	va_start(args, format);
	vsnprintf(buffer, bufferSize, format, args);
	va_end(args);
}