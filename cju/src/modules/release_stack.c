#include "cju/modules/release_stack.h"

CuReleaseStack* cuReleaseStackCreate()
{
	CuReleaseStack* pReleaseStack = (CuReleaseStack*)CU_PLATFORM_API(cuAllocate)(sizeof(CuReleaseStack));
	CU_ASSERT(pReleaseStack != CU_NULL);

	pReleaseStack->pStack = CU_STACK_CREATE(CuReleaseStackEntry);

	return pReleaseStack;
}

void cuReleaseStackPush(CuReleaseStack* pReleaseStack, CuReleaseFunction pReleaseFunction, void* pUserData)
{
	CU_ASSERT(pReleaseStack != CU_NULL);
	CuReleaseStackEntry entry = {
		.pReleaseFunction = pReleaseFunction,
		.pUserData		  = pUserData,
	};

	CU_STACK_PUSH(CuReleaseStackEntry, pReleaseStack->pStack, entry);
}

void cuReleaseStackExecuteAndDestroy(CuReleaseStack* pReleaseStack)
{
	CU_ASSERT(pReleaseStack != CU_NULL);

	while (CU_TRUE)
	{
		u32 stackSize = CU_STACK_COUNT(CuReleaseStackEntry, pReleaseStack->pStack);

		if (stackSize == 0)
		{
			break;
		}

		CuReleaseStackEntry* pEntry = CU_STACK_TOP(CuReleaseStackEntry, pReleaseStack->pStack);

		pEntry->pReleaseFunction(pEntry->pUserData);

		CU_STACK_POP(CuReleaseStackEntry, pReleaseStack->pStack);
	};

	CU_STACK_DESTROY(CuReleaseStackEntry, pReleaseStack->pStack);
	CU_PLATFORM_API(cuFree)(pReleaseStack, sizeof(CuReleaseStack));
}
