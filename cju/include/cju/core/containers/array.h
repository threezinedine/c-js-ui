#pragma once
#include "cju/platform/platform.h"

/**
 * Comprehensive dynamic array structure which holds metadata about the array.
 * This structure is used internally by the CuArray functions.
 */
typedef struct CuArray
{
	void* pData;
	u32	  count;
	u32	  capacity;
	u32	  elementSize;
} CuArray;

/**

 * Initializes a new dynamic array with the specified element size and capacity.
 *
 * @param elementSize The size of each element in the array.
 * @param capacity    The initial capacity of the array.
 *
 * @return A pointer to the newly created CuArray.
 */
CuArray* cuArrayInit(u32 elementSize, u32 capacity);

/**
 * Resizes the dynamic array to the new specified capacity.
 * If the new capacity is smaller than the current capacity, no action is taken.
 *
 * @param pArray      A pointer to the CuArray to resize.
 * @param newCapacity The new capacity for the array.
 */
void cuArrayResize(CuArray* pArray, u32 newCapacity);

/**
 * Frees the memory allocated for the dynamic array.
 *
 * @param pArray A pointer to the CuArray to free.
 */
void cuArrayFree(CuArray* pArray);

/**
 * Macro to define a typed array structure for a specific type T.
 *
 * @param T The type of elements in the array.
 * @param capacity The initial capacity of the array.
 *
 * @return A pointer to the defined array structure.
 */
#define CU_ARRAY_INIT(T, cap) (cuArrayInit(sizeof(T), (cap)))

#if CU_DEBUG
/**
 * Macro to resize a typed array to a new capacity.
 *
 * @param T The type of elements in the array.
 * @param vec A pointer to the CuArray to resize.
 * @param newCap The new capacity for the array.
 */
#define CU_ARRAY_RESIZE(T, vec, newCap)                                                                                \
	do                                                                                                                 \
	{                                                                                                                  \
		CU_ASSERT(sizeof(T) == (vec)->elementSize);                                                                    \
		cuArrayResize((CuArray*)(vec), (newCap));                                                                      \
	} while (0)
#else
#define CU_ARRAY_RESIZE(T, vec, newCap) (cuArrayResize((CuArray*)(vec), (newCap)))
#endif

#if CU_DEBUG
/**
 * Macro to free a typed array.
 *
 * @param T The type of elements in the array.
 * @param vec A pointer to the CuArray to free.
 */
#define CU_ARRAY_FREE(T, vec)                                                                                          \
	do                                                                                                                 \
	{                                                                                                                  \
		CU_ASSERT(sizeof(T) == (vec)->elementSize);                                                                    \
		cuArrayFree((CuArray*)(vec));                                                                                  \
	} while (0)
#else
#define CU_ARRAY_FREE(T, vec) (cuArrayFree(vec))
#endif