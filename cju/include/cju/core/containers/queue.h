#pragma once

#include "cju/platform/platform.h"
#include "list.h"

/**
 * @file queue.h
 * @brief Queue container implementation using linked list.
 */
typedef struct CuQueue
{
	CuList* pList; ///< Underlying list to store queue elements
} CuQueue;

#if CU_DEBUG
/**
 * Creates a new queue.
 * @return Pointer to the created queue.
 */
CuQueue* cuQueueCreate(const char* pTypeName);
#else
CuQueue* cuQueueCreate();
#endif

/**
 * Pushes data onto the queue.
 * @param pQueue Pointer to the queue.
 * @param pData Pointer to the data to push.
 * @param size Size of the data to push.
 */
void cuQueuePush(CuQueue* pQueue, void* pData, u32 size);

/**
 * Get the top element of the queue without removing it.
 * @param pQueue Pointer to the queue.
 * @return Pointer to the top element of the queue.
 */
void* cuQueueFront(CuQueue* pQueue);

/**
 * Pops the top element off the queue.
 * @param pQueue Pointer to the queue.
 */
void cuQueuePop(CuQueue* pQueue);

/**
 * Destroys the given queue.
 * @param pQueue Pointer to the queue to destroy.
 */
void cuQueueDestroy(CuQueue* pQueue);

#if CU_DEBUG
/**
 * Macro to create a new queue for a specific type.
 * @param type The type of elements in the queue.
 * @return Pointer to the created queue.
 */
#define CU_QUEUE_CREATE(type) cuQueueCreate(#type)
#else // CU_DEBUG
#define CU_QUEUE_CREATE(type) cuQueueCreate()
#endif // CU_DEBUG

#if CU_DEBUG
#define CU_QUEUE_TYPE_ASSERT(type, queue)                                                                              \
	do                                                                                                                 \
	{                                                                                                                  \
		if (cuStrEqual(#type, (queue)->pList->pTypeName) == CU_FALSE)                                                  \
		{                                                                                                              \
			CU_RAISE_EXCEPTION(CU_EXCEPTION_CODE_INVALID_TYPE,                                                         \
							   "Type mismatch: expected queue of type '%s', but got type '%s'",                        \
							   (queue)->pList->pTypeName,                                                              \
							   #type);                                                                                 \
		}                                                                                                              \
	} while (CU_FALSE)
#else
#define CU_QUEUE_TYPE_ASSERT(queue, type)
#endif

/**
 * Macro to push data onto the queue.
 * @param type The type of elements in the queue.
 * @param queue Pointer to the queue.
 * @param data Pointer to the data to push.
 */
#define CU_QUEUE_PUSH(type, queue, data)                                                                               \
	do                                                                                                                 \
	{                                                                                                                  \
		CU_QUEUE_TYPE_ASSERT(type, queue);                                                                             \
		type _data = (data);                                                                                           \
		cuQueuePush((queue), &_data, sizeof(type));                                                                    \
	} while (CU_FALSE)

/**
 * Macro to get the size of the queue.
 * @param type The type of elements in the queue.
 * @param queue Pointer to the queue.
 * @return Number of elements in the queue.
 */
#define CU_QUEUE_COUNT(type, queue)                                                                                    \
	((queue)->pList->count);                                                                                           \
	do                                                                                                                 \
	{                                                                                                                  \
		CU_QUEUE_TYPE_ASSERT(type, queue);                                                                             \
	} while (CU_FALSE)

/**
 * Macro to get the top element of the queue.
 * @param type The type of elements in the queue.
 * @param queue Pointer to the queue.
 * @return Pointer to the top element of the queue.
 */
#define CU_QUEUE_FRONT(type, queue)                                                                                    \
	((type*)(cuQueueFront(queue)));                                                                                    \
	do                                                                                                                 \
	{                                                                                                                  \
		CU_QUEUE_TYPE_ASSERT(type, queue);                                                                             \
	} while (CU_FALSE)

/**
 * Macro to pop the top element off the queue.
 * @param type The type of elements in the queue.
 * @param queue Pointer to the queue.
 * @return Pointer to the popped element.
 */
#define CU_QUEUE_POP(type, queue)                                                                                      \
	do                                                                                                                 \
	{                                                                                                                  \
		CU_QUEUE_TYPE_ASSERT(type, queue);                                                                             \
		cuQueuePop(queue);                                                                                             \
	} while (CU_FALSE)

/**
 * Macro to destroy the given queue.
 * @param type The type of elements in the queue.
 * @param queue Pointer to the queue to destroy.
 */
#define CU_QUEUE_DESTROY(type, queue)                                                                                  \
	do                                                                                                                 \
	{                                                                                                                  \
		CU_QUEUE_TYPE_ASSERT(type, queue);                                                                             \
		cuQueueDestroy((queue));                                                                                       \
	} while (CU_FALSE)
