#include "cju/cju.h"
#include <stdio.h>
#include <stdlib.h>

void* allocate(size_t size)
{
	return malloc(size);
}

void deallocate(void* ptr)
{
	free(ptr);
}

int main(void)
{
	CU_PLATFORM_API_SET(cuAllocate, allocate);
	CU_PLATFORM_API_SET(cuFree, deallocate);

	cuPrintLn("Using custom memory allocation functions.");

	void* ptr = CU_PLATFORM_API(cuAllocate)(128);

	CU_PLATFORM_API(cuMemoryAssertNoLeaks)();

	return 0;
}