#pragma once

typedef unsigned char	   u8;
typedef unsigned short	   u16;
typedef unsigned int	   u32;
typedef unsigned long long u64;

typedef char	  i8;
typedef short	  i16;
typedef int		  i32;
typedef long long i64;

typedef float  f32;
typedef double f64;

typedef u8 b8;

#define CU_NULL	 (void*)0
#define CU_TRUE	 1
#define CU_FALSE 0

#define CU_ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

#define CU_KB(size) ((size) * 1024)
#define CU_MB(size) (CU_KB(size) * 1024)

#if CU_PLATFORM_WINDOWS
#error "Windows platform is not yet supported"
#elif CU_PLATFORM_UNIX || CU_PLATFORM_WEB
#include <signal.h>
#define debugbreak() __builtin_trap()
#else
#error "Platform not supported"
#endif

#if CU_DEBUG
#define CU_ASSERT(cond)                                                                                                \
	do                                                                                                                 \
	{                                                                                                                  \
		if (!(cond))                                                                                                   \
		{                                                                                                              \
			cuConsoleSetColor(CU_CONSOLE_COLOR_RED);                                                                   \
			cuConsolePrintFormat("Assertion failed: %s, file %s, line %d\n", #cond, __FILE__, __LINE__);               \
			cuConsoleSetColor(CU_CONSOLE_COLOR_RESET);                                                                 \
			debugbreak();                                                                                              \
		}                                                                                                              \
	} while (CU_FALSE)

#define CU_ASSERT_MSG(cond, msg, ...)                                                                                  \
	do                                                                                                                 \
	{                                                                                                                  \
		if (!(cond))                                                                                                   \
		{                                                                                                              \
			cuConsoleSetColor(CU_CONSOLE_COLOR_RED);                                                                   \
			char buffer[512];                                                                                          \
			cuBufferedString(buffer, sizeof(buffer), msg, ##__VA_ARGS__);                                              \
			cuConsolePrintFormat("Assertion failed: message: %s, file %s, line %d\n", buffer, __FILE__, __LINE__);     \
			cuConsoleSetColor(CU_CONSOLE_COLOR_RESET);                                                                 \
			debugbreak();                                                                                              \
		}                                                                                                              \
	} while (CU_FALSE)
#else
#define CU_ASSERT(cond)
#define CU_ASSERT_MSG(cond, msg)
#endif

#define CU_NOT_IMPLEMENTED() CU_ASSERT_MSG(CU_FALSE, "Method %s not yet implemented", __FUNCTION__)

#define CU_UNUSED(var) (void)(var)

#include "console.h"
#include "memory.h"