#include "cju/platform/platform.h"
#include <stdlib.h>

const char* cuConvertExceptionCodeToString(CuExceptionCode code)
{
	switch (code)
	{
	case CU_EXCEPTION_CODE_UNKNOWN:
		return "UNKNOWN";
	case CU_EXCEPTION_CODE_INDEX_OUT_OF_BOUNDS:
		return "INDEX_OUT_OF_BOUNDS";
	case CU_EXCEPTION_CODE_INVALID_TYPE:
		return "INVALID_TYPE";
	case CU_EXCEPTION_CODE_INVALID_OWNER:
		return "INVALID_OWNER";
	// Add more cases as needed.
	default:
		CU_UNREACHABLE();
	}
}

void cuRaiseException(CuExceptionCode code, const char* message, const char* file, u32 line)
{
#if !CU_TESTING
	// For now, just print the exception details and abort.
	cuConsoleSetColor(CU_CONSOLE_COLOR_RED);
	cuPrintLnFormat("Exception raised: code=%s, message=%s, file=%s, line=%u",
					cuConvertExceptionCodeToString(code),
					message,
					file,
					line);
	cuConsoleSetColor(CU_CONSOLE_COLOR_RESET);
	// In a real implementation, you might want to log this or handle it differently.
#endif
	exit(code);
}