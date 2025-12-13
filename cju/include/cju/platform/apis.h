#pragma once

#if __cplusplus
extern "C" {
#endif
#include "common.h"

/**
 * @file apis.h
 * All the possible platform API methods which the user can override it,
 * such as memory allocation, threading, timing, etc. If the user does not
 * provide an implementation, the default one will be used (support Linux, Windows, Web).
 */

#define CU_PLATFORM_API_ATTRIBUTE(api) FPN_##api api;

/**
 * Structure holding function pointers for platform-specific API implementations.
 * Users can provide their own implementations for these functions to override
 * the default behavior.
 */
typedef struct CuPlatformAPI
{
	// memory.h
	CU_PLATFORM_API_ATTRIBUTE(cuAllocate)
	CU_PLATFORM_API_ATTRIBUTE(cuFree)
	CU_PLATFORM_API_ATTRIBUTE(cuMemorySet)
	CU_PLATFORM_API_ATTRIBUTE(cuMemoryCopy)
	CU_PLATFORM_API_ATTRIBUTE(cuMemoryAssertNoLeaks)
} CuPlatformAPI;

extern CuPlatformAPI gCuPlatformAPI;

#define CU_PLATFORM_API_SET(api, func) (gCuPlatformAPI.api = (FPN_##api)func)

#define CU_PLATFORM_API(api) (gCuPlatformAPI.api == CU_NULL ? api##_default : gCuPlatformAPI.api)

#if __cplusplus
}
#endif