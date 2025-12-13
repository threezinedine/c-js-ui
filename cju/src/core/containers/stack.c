#include "cju/core/containers/containers.h"

#if CU_DEBUG
CuStack* cuStackCreate(const char* pTypeName)
#else  // CU_DEBUG
CuStack* cuStackCreate()
#endif // CU_DEBUG
{
	CuStack* pStack = (CuStack*)cuAllocate(sizeof(CuStack));
	CU_ASSERT(pStack != CU_NULL);

#if CU_DEBUG
	pStack->pList = cuListCreate(pTypeName);
#else  // CU_DEBUG
	pStack->pList = cuListCreate();
#endif // CU_DEBUG

	return pStack;
}

void cuStackPush(CuStack* pStack, void* pData, u32 size)
{
	CU_ASSERT(pStack != CU_NULL);
	CU_ASSERT(pStack->pList != CU_NULL);
	CU_ASSERT(pData != CU_NULL);

	cuListAppend(pStack->pList, pData, size);
}

void* cuStackTop(CuStack* pStack)
{
	CU_ASSERT(pStack != CU_NULL);
	CU_ASSERT(pStack->pList != CU_NULL);

	if (pStack->pList->count == 0)
	{
		CU_RAISE_EXCEPTION(CU_EXCEPTION_CODE_INDEX_OUT_OF_BOUNDS, "Attempting to access top of an empty stack");
	}

	CU_ASSERT(pStack->pList->pTail != CU_NULL);

	return pStack->pList->pTail->pData;
}

void cuStackPop(CuStack* pStack)
{
	CU_ASSERT(pStack != CU_NULL);
	CU_ASSERT(pStack->pList != CU_NULL);

	if (pStack->pList->count == 0)
	{
		CU_RAISE_EXCEPTION(CU_EXCEPTION_CODE_INDEX_OUT_OF_BOUNDS, "Attempting to pop from an empty stack");
	}

	CU_ASSERT(pStack->pList->pTail != CU_NULL);

	CuListNode* pTailNode = pStack->pList->pTail;
	void*		pData	  = pTailNode->pData;

	cuListRemoveNode(pStack->pList, pTailNode);
}

void cuStackDestroy(CuStack* pStack)
{
	CU_ASSERT(pStack != CU_NULL);
	CU_ASSERT(pStack->pList != CU_NULL);

	cuListDestroy(pStack->pList);
	cuFree(pStack, sizeof(CuStack));
}