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
 * Adds a new element to the end of the dynamic array.
 * If the array is full, it will be resized to accommodate the new element.
 *
 * @param pArray   A pointer to the CuArray to which the element will be added.
 * @param pElement A pointer to the element to add to the array.
 */
void cuArrayPushBack(CuArray* pArray, const void* pElement);

/**
 * Retrieves a pointer to the element at the specified index in the dynamic array.
 *
 * @param pArray A pointer to the CuArray from which to retrieve the element.
 * @param index  The index of the element to retrieve.
 *
 * @return A pointer to the element at the specified index.
 */
void* cuArrayGet(CuArray* pArray, u32 index);

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
#define CU_ARRAY_TYPE_ASSERT(T, vec)                                                                                   \
	do                                                                                                                 \
	{                                                                                                                  \
		CU_ASSERT(sizeof(T) == (vec)->elementSize);                                                                    \
	} while (CU_FALSE)
#else
#define CU_ARRAY_TYPE_ASSERT(T, vec)
#endif

/**
 * Macro to add a new element to the end of a typed array.
 *
 * @param T The type of elements in the array.
 * @param vec A pointer to the CuArray to which the element will be added.
 * @param value The value of the element to add to the array.
 */
#define CU_ARRAY_PUSH_BACK(T, vec, value)                                                                              \
	do                                                                                                                 \
	{                                                                                                                  \
		CU_ARRAY_TYPE_ASSERT(T, vec);                                                                                  \
		T rightValue = value;                                                                                          \
		cuArrayPushBack((CuArray*)(vec), &rightValue);                                                                 \
	} while (CU_FALSE)

#define CU_ARRAY_GET(T, vec, index)                                                                                    \
	((T*)((u8*)(vec)->pData + ((index) * sizeof(T))));                                                                 \
	do                                                                                                                 \
	{                                                                                                                  \
		CU_ARRAY_TYPE_ASSERT(T, vec);                                                                                  \
	} while (CU_FALSE)

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
		CU_ARRAY_TYPE_ASSERT(T, vec);                                                                                  \
		cuArrayResize((CuArray*)(vec), (newCap));                                                                      \
	} while (CU_FALSE)

/**
 * Macro to free a typed array.
 *
 * @param T The type of elements in the array.
 * @param vec A pointer to the CuArray to free.
 */
#define CU_ARRAY_FREE(T, vec)                                                                                          \
	do                                                                                                                 \
	{                                                                                                                  \
		CU_ARRAY_TYPE_ASSERT(T, vec);                                                                                  \
		cuArrayFree((CuArray*)(vec));                                                                                  \
	} while (CU_FALSE)
