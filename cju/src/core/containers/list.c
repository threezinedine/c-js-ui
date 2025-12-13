#include "cju/core/containers/containers.h"

#if CU_DEBUG
CuList* cuListCreate(const char* pTypeName)
#else
CuList* cuListCreate()
#endif
{
	CuList* pList = (CuList*)cuAllocate(sizeof(CuList));
	CU_ASSERT(pList != CU_NULL);

	pList->pHead = CU_NULL;
	pList->pTail = CU_NULL;
	pList->count = 0;

#if CU_DEBUG
	pList->pTypeName = pTypeName;
#endif

	return pList;
}

void cuListAppend(CuList* pList, const void* pData, u32 dataSize)
{
	CU_ASSERT(pList != CU_NULL);
	CU_ASSERT(pData != CU_NULL);
	CU_ASSERT(dataSize > 0);

	CuListNode* pNewNode = (CuListNode*)cuAllocate(sizeof(CuListNode));
	CU_ASSERT(pNewNode != CU_NULL);

	pNewNode->pData = cuAllocate(dataSize);
	CU_ASSERT(pNewNode->pData != CU_NULL);

	cuMemoryCopy(pNewNode->pData, pData, dataSize);
	pNewNode->dataSize	= dataSize;
	pNewNode->pNext		= CU_NULL;
	pNewNode->pPrevious = pList->pTail;

#if CU_DEBUG
	pNewNode->pOwnerList = pList;
#endif

	if (pList->pTail != CU_NULL)
	{
		pList->pTail->pNext = pNewNode;
	}
	else
	{
		pList->pHead = pNewNode;
	}

	pList->pTail = pNewNode;
	pList->count++;
}

void* cuListGetAt(CuList* pList, u32 index)
{
	CU_ASSERT(pList != CU_NULL);

	if (index >= pList->count)
	{
		CU_RAISE_EXCEPTION(CU_EXCEPTION_CODE_INDEX_OUT_OF_BOUNDS,
						   "Index %u is out of bounds for linked list of size %u",
						   index,
						   pList->count);
	}

	CuListNode* pCurrent = pList->pHead;
	for (u32 i = 0; i < index; i++)
	{
		pCurrent = pCurrent->pNext;
	}

	return pCurrent->pData;
}

void cuListRemoveNode(CuList* pList, CuListNode* pNode)
{
	CU_ASSERT(pList != CU_NULL);
	CU_ASSERT(pNode != CU_NULL);

	CuListNode* pPrevious = pNode->pPrevious;
	CuListNode* pNext	  = pNode->pNext;

	if (pPrevious != CU_NULL)
	{
		pPrevious->pNext = pNext;
	}
	else
	{
		pList->pHead = pNext;
	}

	if (pNext != CU_NULL)
	{
		pNext->pPrevious = pPrevious;
	}
	else
	{
		pList->pTail = pPrevious;
	}

	CU_ASSERT(pNode->pData != CU_NULL);
	cuFree(pNode->pData, pNode->dataSize);
	cuFree(pNode, sizeof(CuListNode));
	pList->count--;
}

void cuListInsertNodeAfter(CuList* pList, CuListNode* pNode, const void* pData, u32 dataSize)
{
	CU_ASSERT(pList != CU_NULL);
	CU_ASSERT(pNode != CU_NULL);
	CU_ASSERT(pData != CU_NULL);
	CU_ASSERT(dataSize > 0);

	CuListNode* pNewNode = (CuListNode*)cuAllocate(sizeof(CuListNode));
	CU_ASSERT(pNewNode != CU_NULL);

	pNewNode->pData = cuAllocate(dataSize);
	CU_ASSERT(pNewNode->pData != CU_NULL);

	cuMemoryCopy(pNewNode->pData, pData, dataSize);
	pNewNode->dataSize	= dataSize;
	pNewNode->pPrevious = pNode;
	pNewNode->pNext		= pNode->pNext;

	if (pNode->pNext != CU_NULL)
	{
		pNode->pNext->pPrevious = pNewNode;
	}
	else
	{
		pList->pTail = pNewNode;
	}

	pNode->pNext = pNewNode;
	pList->count++;
}

void cuListInsertNodeBefore(CuList* pList, CuListNode* pNode, const void* pData, u32 dataSize)
{
	CU_ASSERT(pList != CU_NULL);
	CU_ASSERT(pNode != CU_NULL);
	CU_ASSERT(pData != CU_NULL);
	CU_ASSERT(dataSize > 0);

	CuListNode* pNewNode = (CuListNode*)cuAllocate(sizeof(CuListNode));
	CU_ASSERT(pNewNode != CU_NULL);

	pNewNode->pData = cuAllocate(dataSize);
	CU_ASSERT(pNewNode->pData != CU_NULL);

	cuMemoryCopy(pNewNode->pData, pData, dataSize);
	pNewNode->dataSize	= dataSize;
	pNewNode->pNext		= pNode;
	pNewNode->pPrevious = pNode->pPrevious;

	if (pNode->pPrevious != CU_NULL)
	{
		pNode->pPrevious->pNext = pNewNode;
	}
	else
	{
		pList->pHead = pNewNode;
	}

	pNode->pPrevious = pNewNode;
	pList->count++;
}

void cuListClear(CuList* pList)
{
	CU_ASSERT(pList != CU_NULL);

	CuListNode* pCurrent = pList->pHead;

	while (pCurrent != CU_NULL)
	{
		CuListNode* pNext = pCurrent->pNext;
		cuListRemoveNode(pList, pCurrent);
		pCurrent = pNext;
	}

	pList->pHead = CU_NULL;
	pList->pTail = CU_NULL;
	pList->count = 0;
}

void cuListDestroy(CuList* pList)
{
	CU_ASSERT(pList != CU_NULL);

	cuListClear(pList);
	cuFree(pList, sizeof(CuList));
}