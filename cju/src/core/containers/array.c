#include "cju/core/containers/containers.h"

#if CU_DEBUG
CuArray* cuArrayInit(u32 elementSize, u32 capacity, const char* pTypeName)
#else
CuArray* cuArrayInit(u32 elementSize, u32 capacity)
#endif
{
	CuArray* pArray = (CuArray*)cuAllocate(sizeof(CuArray));
	CU_ASSERT(pArray != CU_NULL);

	pArray->elementSize = elementSize;
	pArray->capacity	= capacity;
	pArray->count		= 0;
	pArray->pData		= cuAllocate(elementSize * capacity);

#if CU_DEBUG
	pArray->pTypeName = pTypeName;
#endif
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

void cuArrayPushBack(CuArray* pArray, const void* pElement)
{
	CU_ASSERT(pArray != CU_NULL);
	CU_ASSERT(pElement != CU_NULL);

	if (pArray->count >= pArray->capacity)
	{
		u32 newCapacity = pArray->capacity == 0 ? 1 : pArray->capacity * 2;
		cuArrayResize(pArray, newCapacity);
	}

	u8* pDest = (u8*)pArray->pData + (pArray->count * pArray->elementSize);
	cuMemoryCopy(pDest, pElement, pArray->elementSize);
	pArray->count++;
}

void cuArrayInsert(CuArray* pArray, u32 index, const void* pElement)
{
	CU_ASSERT(pArray != CU_NULL);
	CU_ASSERT(pElement != CU_NULL);

	if (index == pArray->count)
	{
		cuArrayPushBack(pArray, pElement);
		return;
	}

	if (index > pArray->count)
	{
		CU_RAISE_EXCEPTION(CU_EXCEPTION_CODE_INDEX_OUT_OF_BOUNDS,
						   "Index %u is out of bounds for array of size %u",
						   index,
						   pArray->count);
	}

	if (pArray->count == pArray->capacity)
	{
		u32 newCapacity = pArray->capacity == 0 ? 1 : pArray->capacity * 2;
		cuArrayResize(pArray, newCapacity);
	}

	for (i32 i = pArray->count - 1; i >= (i32)index; --i)
	{
		u8* pDest = (u8*)(pArray->pData + (i * pArray->elementSize));
		u8* pSrc  = (u8*)(pArray->pData + ((i + 1) * pArray->elementSize));
		cuMemoryCopy(pDest, pSrc, pArray->elementSize);
	}

	cuMemoryCopy(cuArrayGet(pArray, index), pElement, pArray->elementSize);
	pArray->count++;
}

void cuArrayRemoveAt(CuArray* pArray, u32 index)
{
	CU_ASSERT(pArray != CU_NULL);

	if (index >= pArray->count)
	{
		CU_RAISE_EXCEPTION(CU_EXCEPTION_CODE_INDEX_OUT_OF_BOUNDS,
						   "Index %u is out of bounds for array of size %u",
						   index,
						   pArray->count);
	}

	for (u32 i = index; i < pArray->count - 1; i++)
	{
		void* pSrc	= cuArrayGet(pArray, i + 1);
		void* pDest = cuArrayGet(pArray, i);
		cuMemoryCopy(pDest, pSrc, pArray->elementSize);
	}
	pArray->count--;
}

void* cuArrayGet(CuArray* pArray, u32 index)
{
	CU_ASSERT(pArray != CU_NULL);

	if (index >= pArray->count)
	{
		CU_RAISE_EXCEPTION(CU_EXCEPTION_CODE_INDEX_OUT_OF_BOUNDS,
						   "Index %u is out of bounds for array of size %u",
						   index,
						   pArray->count);
	}

	return (u8*)pArray->pData + (index * pArray->elementSize);
}

void cuArrayClear(CuArray* pArray)
{
	CU_ASSERT(pArray != CU_NULL);
	pArray->count = 0;
}

void cuArrayFree(CuArray* pArray)
{
	CU_ASSERT(pArray != CU_NULL);

	cuFree(pArray->pData, pArray->elementSize * pArray->capacity);
	cuFree(pArray, sizeof(CuArray));
}