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

void debugbreak();

#define CU_BUFFER_SIZE 1024

#if CU_DEBUG
#define CU_ASSERT(cond)                                                                                                \
	do                                                                                                                 \
	{                                                                                                                  \
		if (!(cond))                                                                                                   \
		{                                                                                                              \
			cuConsoleSetColor(CU_CONSOLE_COLOR_RED);                                                                   \
			cuPrintFormat("Assertion failed: %s, file %s, line %d\n", #cond, __FILE__, __LINE__);                      \
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
			char buffer[CU_BUFFER_SIZE];                                                                               \
			cuBufferedString(buffer, sizeof(buffer), msg, ##__VA_ARGS__);                                              \
			cuPrintFormat("Assertion failed: message: %s, file %s, line %d\n", buffer, __FILE__, __LINE__);            \
			cuConsoleSetColor(CU_CONSOLE_COLOR_RESET);                                                                 \
			debugbreak();                                                                                              \
		}                                                                                                              \
	} while (CU_FALSE)

#define CU_UNREACHABLE()                                                                                               \
	do                                                                                                                 \
	{                                                                                                                  \
		cuConsoleSetColor(CU_CONSOLE_COLOR_RED);                                                                       \
		cuPrintFormat("Unreachable code reached: file %s, line %d\n", __FILE__, __LINE__);                             \
		cuConsoleSetColor(CU_CONSOLE_COLOR_RESET);                                                                     \
		debugbreak();                                                                                                  \
	} while (CU_FALSE)
#else
#define CU_ASSERT(cond)
#define CU_ASSERT_MSG(cond, msg, ...)
#define CU_UNREACHABLE()
#endif

#define CU_NOT_IMPLEMENTED() CU_ASSERT_MSG(CU_FALSE, "Method %s not yet implemented", __FUNCTION__)

#define CU_UNUSED(var) (void)(var)

#include "console.h"
#include "exeptions.h"
#include "memory.h"
#include "string.h"