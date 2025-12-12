#include "cju/core/containers/containers.h"

CuArray* cuArrayInit(u32 elementSize, u32 capacity)
{
	CuArray* pArray = (CuArray*)cuAllocate(sizeof(CuArray));
	CU_ASSERT(pArray != CU_NULL);

	pArray->elementSize = elementSize;
	pArray->capacity	= capacity;
	pArray->count		= 0;
	pArray->pData		= cuAllocate(elementSize * capacity);
	return pArray;
}

void cuArrayResize(CuArray* pArray, u32 newCapacity)
{
	CU_ASSERT(pArray != CU_NULL);

	if (newCapacity <= pArray->capacity)
	{
		return;
	}

	void* pNewData = cuAllocate(pArray->elementSize * newCapacity);
	cuMemoryCopy(pNewData, pArray->pData, pArray->elementSize * pArray->count);

	cuFree(pArray->pData, pArray->elementSize * pArray->capacity);

	pArray->pData	 = pNewData;
	pArray->capacity = newCapacity;
}

void cuArrayFree(CuArray* pArray)
{
	CU_ASSERT(pArray != CU_NULL);

	cuFree(pArray->pData, pArray->elementSize * pArray->capacity);
	cuFree(pArray, sizeof(CuArray));
}