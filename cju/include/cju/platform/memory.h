#pragma once

#include "common.h"

/**
 * The forward declaration of TraceInfo structure. Each platform-specific
 * implementation may define its own version of this structure.
 */
struct TraceInfo;

typedef struct TraceInfo TraceInfo;

/**
 * Allocates a block of memory of the specified size.
 *
 * @param size The size of memory to allocate in bytes.
 *
 * @return A pointer to the allocated memory block.
 */
void* cuAllocate(u32 size);

/**
 * Frees a previously allocated block of memory.
 *
 * @param ptr  A pointer to the memory block to free.
 * @param size The size of the memory block to free in bytes.
 */
void cuFree(void* ptr, u32 size);

/**
 * Sets a block of memory to a specified value.
 *
 * @param dest  A pointer to the memory block to set.
 * @param value The value to set each byte of the memory block to.
 * @param size  The size of the memory block to set in bytes.
 */
void cuMemorySet(void* dest, u8 value, u32 size);

/**
 * Copies a block of memory from a source to a destination.
 *
 * @param dest A pointer to the destination memory block.
 * @param src  A pointer to the source memory block.
 * @param size The size of the memory block to copy in bytes.
 */
void cuMemoryCopy(void* dest, const void* src, u32 size);

/**
 * Asserts that there are no memory leaks.
 * This function can be used in debug builds to verify that all allocated memory has been freed.
 */
void cuMemoryAssertNoLeaks();

/**
 * Retrieves the current trace information.
 *
 * @param pOutTraceInfo A pointer to a TraceInfo structure to be filled with the current trace data.
 */
void cuGetCurrentTrace(TraceInfo* pOutTraceInfo);

/**
 * Prints the trace information for debugging purposes.
 *
 * @param pTraceInfo A pointer to the TraceInfo structure containing the trace data, not NULL.
 */
void cuPrintTrace(const TraceInfo* pTraceInfo);

#define CU_CURRENT_TRACE()                                                                                             \
	do                                                                                                                 \
	{                                                                                                                  \
		TraceInfo traceInfo;                                                                                           \
		cuGetCurrentTrace(&traceInfo);                                                                                 \
		cuPrintTrace(&traceInfo);                                                                                      \
	} while (CU_FALSE)
