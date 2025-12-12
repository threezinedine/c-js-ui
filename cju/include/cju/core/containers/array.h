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

#if CU_DEBUG
	const char* pTypeName; ///< Used for debugging purposes, for checking type consistency
#endif
} CuArray;

#if CU_DEBUG
/**
 * Initializes a new dynamic array with the specified element size and capacity.
 *
 * @param elementSize The size of each element in the array.
 * @param capacity    The initial capacity of the array.
 * @param pTypeName   The name of the type stored in the array (for debugging purposes).
 *
 * @return A pointer to the newly created CuArray.
 */
CuArray* cuArrayInit(u32 elementSize, u32 capacity, const char* pTypeName);
#else
/**
 * Initializes a new dynamic array with the specified element size and capacity.
 *
 * @param elementSize The size of each element in the array.
 * @param capacity    The initial capacity of the array.
 *
 * @return A pointer to the newly created CuArray.
 */
CuArray* cuArrayInit(u32 elementSize, u32 capacity);
#endif

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
 * Inserts a new element at the specified index in the dynamic array.
 * Elements at and after the index are shifted to the right.
 * If the array is full, it will be resized to accommodate the new element.
 *
 * @param pArray   A pointer to the CuArray in which the element will be inserted.
 * @param index  The index of the element to remove. If the index is out of bounds, an exception is raised.
 * the index must be less than or equal to count (== count => push back).
 * @param pElement A pointer to the element to insert into the array.
 */
void cuArrayInsert(CuArray* pArray, u32 index, const void* pElement);

/**
 * Removes the element at the specified index from the dynamic array.
 * Elements after the index are shifted to the left.
 *
 * @param pArray A pointer to the CuArray from which the element will be removed.
 * @param index  The index of the element to remove. If the index is out of bounds, an exception is raised.
 * the index must be less than or equal to count (== count => push back).
 */
void cuArrayRemoveAt(CuArray* pArray, u32 index);

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
 * Clears the dynamic array by setting its count to zero.
 * The capacity remains unchanged, and the allocated memory is not freed.
 *
 * @param pArray A pointer to the CuArray to clear.
 */
void cuArrayClear(CuArray* pArray);

/**
 * Frees the memory allocated for the dynamic array.
 *
 * @param pArray A pointer to the CuArray to free.
 */
void cuArrayFree(CuArray* pArray);

#if CU_DEBUG
/**
 * Macro to define a typed array structure for a specific type T.
 *
 * @param T The type of elements in the array.
 * @param capacity The initial capacity of the array.
 *
 * @return A pointer to the defined array structure.
 */
#define CU_ARRAY_INIT(T, cap) (cuArrayInit(sizeof(T), (cap), #T))
#else
#define CU_ARRAY_INIT(T, cap) (cuArrayInit(sizeof(T), (cap)))
#endif

#if CU_DEBUG
#define CU_ARRAY_TYPE_ASSERT(T, vec)                                                                                   \
	do                                                                                                                 \
	{                                                                                                                  \
		if (cuStrEqual(#T, (vec)->pTypeName) == CU_FALSE)                                                              \
		{                                                                                                              \
			CU_RAISE_EXCEPTION(CU_EXCEPTION_CODE_INVALID_TYPE,                                                         \
							   "Type mismatch: expected array of type '%s', but got type '%s'",                        \
							   (vec)->pTypeName,                                                                       \
							   #T);                                                                                    \
		}                                                                                                              \
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

/**
 * Macro to insert a new element at the specified index in a typed array.
 *
 * @param T The type of elements in the array.
 * @param vec A pointer to the CuArray in which the element will be inserted.
 * @param index The index at which to insert the new element.
 * @param value The value of the element to insert into the array.
 */
#define CU_ARRAY_INSERT(T, vec, index, value)                                                                          \
	do                                                                                                                 \
	{                                                                                                                  \
		CU_ARRAY_TYPE_ASSERT(T, vec);                                                                                  \
		T rightValue = value;                                                                                          \
		cuArrayInsert((CuArray*)(vec), (index), &rightValue);                                                          \
	} while (CU_FALSE)

/**
 * Macro to remove the element at the specified index from a typed array.
 * @param T The type of elements in the array.
 * @param vec A pointer to the CuArray from which the element will be removed.
 * @param index The index of the element to remove.
 */
#define CU_ARRAY_REMOVE_AT(T, vec, index)                                                                              \
	do                                                                                                                 \
	{                                                                                                                  \
		CU_ARRAY_TYPE_ASSERT(T, vec);                                                                                  \
		cuArrayRemoveAt((CuArray*)(vec), (index));                                                                     \
	} while (CU_FALSE)

/**
 * Macro to retrieve a pointer to the element at the specified index in a typed array.
 * @param T The type of elements in the array.
 * @param vec A pointer to the Cu
 * Array from which to retrieve the element.
 * @param index The index of the element to retrieve.
 * @return A pointer to the element at the specified index.
 */
#define CU_ARRAY_GET(T, vec, index)                                                                                    \
	((T*)(cuArrayGet((vec), (index))));                                                                                \
	do                                                                                                                 \
	{                                                                                                                  \
		CU_ARRAY_TYPE_ASSERT(T, vec);                                                                                  \
	} while (CU_FALSE)

#define CU_ARRAY_CLEAR(T, vec)                                                                                         \
	do                                                                                                                 \
	{                                                                                                                  \
		CU_ARRAY_TYPE_ASSERT(T, vec);                                                                                  \
		cuArrayClear((CuArray*)(vec));                                                                                 \
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
