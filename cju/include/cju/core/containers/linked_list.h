#pragma once

#include "cju/platform/platform.h"

typedef struct CU_LinkedListNode
{
	void* pData;
	u32	  dataSize;

	struct CU_LinkedListNode* pNext;
	struct CU_LinkedListNode* pPrevious;
} CU_LinkedListNode;

struct CU_LinkedList
{
	CU_LinkedListNode* pHead;
	CU_LinkedListNode* pTail;

	size_t count;
};

CU_LinkedList* CU_CreateLinkedList();