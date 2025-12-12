#include "cju/platform/platform.h"
#include <stdlib.h>
#include <string.h>

static void* _malloc(u32 size);
static void	 _free(void* ptr, u32 size);

#define MAX_BACKTRACE_DEPTH 16

#if CU_PLATFORM_WINDOWS
struct TraceInfo
{
	// Windows-specific trace information can be added here
};
#elif CU_PLATFORM_UNIX || CU_PLATFORM_WEB
#include <backtrace.h>
#include <execinfo.h>
#include <stdio.h>

// for FILE reading include

struct TraceInfo
{
	void* backtracePtrs[MAX_BACKTRACE_DEPTH]; ///< Array to hold backtrace pointers
	u32	  traceCount;						  ///< Number of valid entries in backtracePtrs
};
#else
#error "Platform not supported"
#endif

/**
 * Structure to track allocated memory blocks for leak detection.
 * This structure is only used in debug builds.
 */
typedef struct MemoryBlock
{
	void*				address;   ///< Address of the allocated memory block
	u32					size;	   ///< Size of the allocated memory block in bytes
	struct MemoryBlock* pNext;	   ///< Pointer to the next memory block in the linked list
	TraceInfo			traceInfo; ///< Trace information at the time of allocation
} MemoryBlock;

/**
 * Total allocated memory in bytes, which is used for tracking memory usage.
 */
static u32 gTotalAllocatedMemoryBytes = 0;

static MemoryBlock* gMemoryBlocksHead = CU_NULL;
void*				cuAllocate(u32 size)
{
	MemoryBlock* pBlock = (MemoryBlock*)_malloc(sizeof(MemoryBlock));
	memset(pBlock, 0, sizeof(MemoryBlock));

	pBlock->address = _malloc(size);
	pBlock->size	= size;

	pBlock->pNext	  = gMemoryBlocksHead;
	gMemoryBlocksHead = pBlock;

	cuGetCurrentTrace(&pBlock->traceInfo);

	return pBlock->address;
}

void cuFree(void* ptr, u32 size)
{
	MemoryBlock* pBlock			= gMemoryBlocksHead;
	MemoryBlock* pPreviousBlock = CU_NULL;

	while (pBlock != CU_NULL)
	{
		if (pBlock->address != ptr)
		{
			pPreviousBlock = pBlock;
			pBlock		   = pBlock->pNext;
		}
		else
		{
			break;
		}
	}

	CU_ASSERT_MSG(pBlock != CU_NULL, "Attempting to free unallocated memory at address %p", ptr);
	CU_ASSERT_MSG(
		pBlock->size == size, "Freeing memory block of size %u, but allocated size was %u", size, pBlock->size);

	if (pPreviousBlock == CU_NULL)
	{
		gMemoryBlocksHead = pBlock->pNext;
	}
	else
	{
		pPreviousBlock->pNext = pBlock->pNext;
	}

	_free(ptr, size);
	_free(pBlock, sizeof(MemoryBlock));
}

void cuMemorySet(void* dest, u8 value, u32 size)
{
	memset(dest, value, size);
}

void cuMemoryCopy(void* dest, const void* src, u32 size)
{
	memcpy(dest, src, size);
}

void cuMemoryAssertNoLeaks()
{
	if (gMemoryBlocksHead != CU_NULL)
	{
		cuConsoleSetColor(CU_CONSOLE_COLOR_RED);
		cuPrintFormat("Memory leaks detected:\n");
		MemoryBlock* pBlock = gMemoryBlocksHead;
		CU_ASSERT(pBlock != CU_NULL);
		{
			cuPrintFormat("Leaked block at address %p of size %u bytes\n", pBlock->address, pBlock->size);
			cuPrintTrace(&pBlock->traceInfo);
			pBlock = pBlock->pNext;
		}
	}

	CU_ASSERT_MSG(
		gTotalAllocatedMemoryBytes == 0, "Total allocated memory bytes is %u, expected 0", gTotalAllocatedMemoryBytes);
}

static void* _malloc(u32 size)
{
	void* ptr = malloc(size);
	gTotalAllocatedMemoryBytes += size;
	return ptr;
}

static void _free(void* ptr, u32 size)
{
	gTotalAllocatedMemoryBytes -= size;
	free(ptr);
}

void cuGetCurrentTrace(TraceInfo* pOutTraceInfo)
{
#if CU_PLATFORM_WINDOWS
	CU_NOT_IMPLEMENTED();
#elif CU_PLATFORM_UNIX || CU_PLATFORM_WEB
	u32 traceCount			  = backtrace(pOutTraceInfo->backtracePtrs, MAX_BACKTRACE_DEPTH);
	pOutTraceInfo->traceCount = traceCount;
#else
#error "Platform not supported"
#endif
}

#if CU_PLATFORM_UNIX || CU_PLATFORM_WEB
static int fullBacktrace(void* data, uintptr_t pc, const char* filename, int lineno, const char* function);
#endif

void cuPrintTrace(const TraceInfo* pTraceInfo)
{
#if CU_PLATFORM_WINDOWS
	CU_NOT_IMPLEMENTED();
#elif CU_PLATFORM_UNIX || CU_PLATFORM_WEB
	struct backtrace_state* pState = backtrace_create_state(NULL, 1, NULL, NULL);
	for (u32 i = 0; i < pTraceInfo->traceCount; ++i)
	{
		backtrace_pcinfo(pState, (size_t)(pTraceInfo->backtracePtrs[i]), fullBacktrace, NULL, NULL);
	}
	debugbreak();
#else
#error "Platform not supported"
#endif
}

#if CU_PLATFORM_UNIX || CU_PLATFORM_WEB
static int fullBacktrace(void* data, uintptr_t pc, const char* filename, int lineno, const char* function)
{
	CuConsoleColor prevColor = CU_CONSOLE_COLOR_DEFAULT;
	cuConsoleSetColor(CU_CONSOLE_COLOR_YELLOW);
	cuPrintFormat("  %p : ", (void*)pc);
	cuConsoleSetColor(prevColor);
	if (function)
	{
		cuPrintFormat("%s", function);
	}
	else
	{
		cuPrintFormat("<unknown>");
	}
	if (filename)
	{
		cuPrintFormat(" at %s:%d\n", filename, lineno);
	}
	else
	{
		cuPrintFormat(" at <unknown>:%d\n", lineno);
	}
}
#endif