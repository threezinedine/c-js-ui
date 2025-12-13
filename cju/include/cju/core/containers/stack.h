#pragma once

#include "cju/platform/platform.h"
#include "list.h"

/**
 * @file stack.h
 * @brief Stack container implementation using linked list.
 */
typedef struct CuStack
{
	CuList* pList; ///< Underlying list to store stack elements
} CuStack;

#if CU_DEBUG
/**
 * Creates a new stack.
 * @return Pointer to the created stack.
 */
CuStack* cuStackCreate(const char* pTypeName);
#else
CuStack* cuStackCreate();
#endif

/**
 * Pushes data onto the stack.
 * @param pStack Pointer to the stack.
 * @param pData Pointer to the data to push.
 * @param size Size of the data to push.
 */
void cuStackPush(CuStack* pStack, void* pData, u32 size);

/**
 * Get the top element of the stack without removing it.
 * @param pStack Pointer to the stack.
 * @return Pointer to the top element of the stack.
 */
void* cuStackTop(CuStack* pStack);

/**
 * Pops the top element off the stack.
 * @param pStack Pointer to the stack.
 */
void cuStackPop(CuStack* pStack);

/**
 * Destroys the given stack.
 * @param pStack Pointer to the stack to destroy.
 */
void cuStackDestroy(CuStack* pStack);

#if CU_DEBUG
/**
 * Macro to create a new stack for a specific type.
 * @param type The type of elements in the stack.
 * @return Pointer to the created stack.
 */
#define CU_STACK_CREATE(type) cuStackCreate(#type)
#else // CU_DEBUG
#define CU_STACK_CREATE(type) cuStackCreate()
#endif // CU_DEBUG

#if CU_DEBUG
#define CU_STACK_TYPE_ASSERT(type, stack)                                                                              \
	do                                                                                                                 \
	{                                                                                                                  \
		if (cuStrEqual(#type, (stack)->pList->pTypeName) == CU_FALSE)                                                  \
		{                                                                                                              \
			CU_RAISE_EXCEPTION(CU_EXCEPTION_CODE_INVALID_TYPE,                                                         \
							   "Type mismatch: expected stack of type '%s', but got type '%s'",                        \
							   (stack)->pList->pTypeName,                                                              \
							   #type);                                                                                 \
		}                                                                                                              \
	} while (CU_FALSE)
#else
#define CU_STACK_TYPE_ASSERT(stack, type)
#endif

/**
 * Macro to push data onto the stack.
 * @param type The type of elements in the stack.
 * @param stack Pointer to the stack.
 * @param data Pointer to the data to push.
 */
#define CU_STACK_PUSH(type, stack, data)                                                                               \
	do                                                                                                                 \
	{                                                                                                                  \
		CU_STACK_TYPE_ASSERT(type, stack);                                                                             \
		type _data = (data);                                                                                           \
		cuStackPush((stack), &_data, sizeof(type));                                                                    \
	} while (CU_FALSE)

/**
 * Macro to get the size of the stack.
 * @param type The type of elements in the stack.
 * @param stack Pointer to the stack.
 * @return Number of elements in the stack.
 */
#define CU_STACK_COUNT(type, stack)                                                                                    \
	((stack)->pList->count);                                                                                           \
	do                                                                                                                 \
	{                                                                                                                  \
		CU_STACK_TYPE_ASSERT(type, stack);                                                                             \
	} while (CU_FALSE)

/**
 * Macro to get the top element of the stack.
 * @param type The type of elements in the stack.
 * @param stack Pointer to the stack.
 * @return Pointer to the top element of the stack.
 */
#define CU_STACK_TOP(type, stack)                                                                                      \
	((type*)(cuStackTop(stack)));                                                                                      \
	do                                                                                                                 \
	{                                                                                                                  \
		CU_STACK_TYPE_ASSERT(type, stack);                                                                             \
	} while (CU_FALSE)

/**
 * Macro to pop the top element off the stack.
 * @param type The type of elements in the stack.
 * @param stack Pointer to the stack.
 * @return Pointer to the popped element.
 */
#define CU_STACK_POP(type, stack)                                                                                      \
	do                                                                                                                 \
	{                                                                                                                  \
		CU_STACK_TYPE_ASSERT(type, stack);                                                                             \
		cuStackPop(stack);                                                                                             \
	} while (CU_FALSE)

/**
 * Macro to destroy the given stack.
 * @param type The type of elements in the stack.
 * @param stack Pointer to the stack to destroy.
 */
#define CU_STACK_DESTROY(type, stack)                                                                                  \
	do                                                                                                                 \
	{                                                                                                                  \
		CU_STACK_TYPE_ASSERT(type, stack);                                                                             \
		cuStackDestroy((stack));                                                                                       \
	} while (CU_FALSE)
