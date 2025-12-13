#include "cju/platform/memory.h"
#include "cju/platform/platform.h"
#include <assert.h>

#if CU_PLATFORM_WINDOWS
#error "Windows platform is not yet supported"
#elif CU_PLATFORM_UNIX || CU_PLATFORM_WEB
#include <signal.h>
#else
#error "Platform not supported"
#endif

static_assert(sizeof(u8) == 1, "u8 must be 1 byte");
static_assert(sizeof(u16) == 2, "u16 must be 2 bytes");
static_assert(sizeof(u32) == 4, "u32 must be 4 bytes");
static_assert(sizeof(u64) == 8, "u64 must be 8 bytes");

static_assert(sizeof(i8) == 1, "i8 must be 1 byte");
static_assert(sizeof(i16) == 2, "i16 must be 2 bytes");
static_assert(sizeof(i32) == 4, "i32 must be 4 bytes");
static_assert(sizeof(i64) == 8, "i64 must be 8 bytes");

static_assert(sizeof(f32) == 4, "f32 must be 4 bytes");
static_assert(sizeof(f64) == 8, "f64 must be 8 bytes");

void debugbreak()
{
#if CU_TESTING
#if CU_PLATFORM_WINDOWS
#error "Windows platform is not yet supported"
#elif CU_PLATFORM_UNIX || CU_PLATFORM_WEB
	CU_RAISE_EXCEPTION(CU_EXCEPTION_CODE_UNKNOWN, "debugbreak called");
#else
#error "Platform not supported"
#endif
#else
#if CU_PLATFORM_WINDOWS
#error "Windows platform is not yet supported"
#elif CU_PLATFORM_UNIX || CU_PLATFORM_WEB
	__builtin_trap();
#else
#error "Platform not supported"
#endif
#endif
}
