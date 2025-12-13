#include "cju/core/containers/containers.h"

#if CU_DEBUG
CuQueue* cuQueueCreate(const char* pTypeName)
#else  // CU_DEBUG
CuQueue* cuQueueCreate()
#endif // CU_DEBUG
{
	CuQueue* pQueue = (CuQueue*)CU_PLATFORM_API(cuAllocate)(sizeof(CuQueue));
	CU_ASSERT(pQueue != CU_NULL);

#if CU_DEBUG
	pQueue->pList = cuListCreate(pTypeName);
#else  // CU_DEBUG
	pQueue->pList = cuListCreate();
#endif // CU_DEBUG

	return pQueue;
}

void cuQueuePush(CuQueue* pQueue, void* pData, u32 size)
{
	CU_ASSERT(pQueue != CU_NULL);
	CU_ASSERT(pQueue->pList != CU_NULL);
	CU_ASSERT(pData != CU_NULL);

	cuListAppend(pQueue->pList, pData, size);
}

void* cuQueueFront(CuQueue* pQueue)
{
	CU_ASSERT(pQueue != CU_NULL);
	CU_ASSERT(pQueue->pList != CU_NULL);

	if (pQueue->pList->count == 0)
	{
		CU_RAISE_EXCEPTION(CU_EXCEPTION_CODE_INDEX_OUT_OF_BOUNDS, "Attempting to access front of an empty queue");
	}

	CU_ASSERT(pQueue->pList->pHead != CU_NULL);

	return pQueue->pList->pHead->pData;
}

void cuQueuePop(CuQueue* pQueue)
{
	CU_ASSERT(pQueue != CU_NULL);
	CU_ASSERT(pQueue->pList != CU_NULL);

	if (pQueue->pList->count == 0)
	{
		CU_RAISE_EXCEPTION(CU_EXCEPTION_CODE_INDEX_OUT_OF_BOUNDS, "Attempting to pop from an empty queue");
	}

	CU_ASSERT(pQueue->pList->pHead != CU_NULL);

	CuListNode* pHeadNode = pQueue->pList->pHead;

	cuListRemoveNode(pQueue->pList, pHeadNode);
}

void cuQueueDestroy(CuQueue* pQueue)
{
	CU_ASSERT(pQueue != CU_NULL);
	CU_ASSERT(pQueue->pList != CU_NULL);

	cuListDestroy(pQueue->pList);
	CU_PLATFORM_API(cuFree)(pQueue, sizeof(CuQueue));
}