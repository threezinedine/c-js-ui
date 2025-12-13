#pragma once

#include "cju/platform/platform.h"

struct CuList;

/**
 * @file linked_list.h
 * @brief A simple doubly linked list implementation.
 */
typedef struct CuListNode
{
	/**
	 * Pointer to the data stored in this node, this struct contains the whole data(copy) not just a pointer
	 */
	void* pData;
	u32	  dataSize; ///< Size of the data stored in pData

	struct CuListNode* pNext;	  ///< Only needed for linked list
	struct CuListNode* pPrevious; ///< Only needed for doubly linked list

#if CU_DEBUG
	struct CuList* pOwnerList; ///< Pointer to the owner list, used for validation
#endif
} CuListNode;

/**
 * @brief A simple doubly linked list structure.
 */
typedef struct CuList
{
	CuListNode* pHead;
	CuListNode* pTail;

	u32 count;

#if CU_DEBUG
	const char* pTypeName; ///< Used for debugging purposes, for checking type consistency
#endif
} CuList;

#if CU_DEBUG
/**
 * @brief Creates a linked list.
 * @param pTypeName The name of the type stored in the linked list (for debugging purposes).
 * @return Pointer to the created linked list.
 */
CuList* cuListCreate(const char* pTypeName);
#else
CuList* cuListCreate();
#endif

/**
 * @brief Appends data to the end of the linked list.
 * @param pList Pointer to the linked list.
 * @param pData Pointer to the data to append.
 * @param dataSize Size of the data to append.
 */
void cuListAppend(CuList* pList, const void* pData, u32 dataSize);

/**
 * @brief Retrieves data at a specific index in the linked list.
 * @param pList Pointer to the linked list.
 * @param index Index of the data to retrieve.
 * @return Pointer to the data at the specified index, or nullptr if index is out of bounds.
 */
void* cuListGetAt(CuList* pList, u32 index);

/**
 * @brief Removes a specific node from the linked list.
 * @param pList Pointer to the linked list.
 * @param pNode Pointer to the node to remove.
 * @return Pointer to the data of the removed node.
 */
void cuListRemoveNode(CuList* pList, CuListNode* pNode);

/**
 * @brief Inserts a new node after the specified node in the linked list.
 * @param pList Pointer to the linked list.
 * @param pNode Pointer to the node after which the new node will be inserted.
 * @param pData Pointer to the data to insert.
 * @param dataSize Size of the data to insert.
 */
void cuListInsertNodeAfter(CuList* pList, CuListNode* pNode, const void* pData, u32 dataSize);

/**
 * @brief Inserts a new node before the specified node in the linked list.
 * @param pList Pointer to the linked list.
 * @param pNode Pointer to the node before which the new node will be inserted.
 * @param pData Pointer to the data to insert.
 * @param dataSize Size of the data to insert.
 */
void cuListInsertNodeBefore(CuList* pList, CuListNode* pNode, const void* pData, u32 dataSize);

/**
 * @brief Clears all nodes in the linked list without destroying the list itself.
 * @param pList Pointer to the linked list to clear.
 */
void cuListClear(CuList* pList);

/**
 * @brief Destroys a linked list and frees all associated memory.
 * @param pList Pointer to the linked list to destroy.
 */
void cuListDestroy(CuList* pList);

#if CU_DEBUG
#define CU_LIST_TYPE_ASSERT(T, list)                                                                                   \
	do                                                                                                                 \
	{                                                                                                                  \
		if (cuStrEqual(#T, (list)->pTypeName) == CU_FALSE)                                                             \
		{                                                                                                              \
			CU_RAISE_EXCEPTION(CU_EXCEPTION_CODE_INVALID_TYPE,                                                         \
							   "Type mismatch: expected linked list of type '%s', but got type '%s'",                  \
							   (list)->pTypeName,                                                                      \
							   #T);                                                                                    \
		}                                                                                                              \
	} while (CU_FALSE)

#define CU_LIST_OWNER_ASSERT(node, list)                                                                               \
	do                                                                                                                 \
	{                                                                                                                  \
		if ((node)->pOwnerList != (list))                                                                              \
		{                                                                                                              \
			CU_RAISE_EXCEPTION(CU_EXCEPTION_CODE_INVALID_OWNER, "Node does not belong to the specified linked list."); \
		}                                                                                                              \
	} while (CU_FALSE)
#else
#define CU_LIST_TYPE_ASSERT(T, list)
#define CU_LIST_OWNER_ASSERT(node, list)
#endif

#if CU_DEBUG
/**
 * @brief Macro to create a typed linked list for a specific type T.
 *
 * @param T The type of elements in the linked list.
 *
 * @return A pointer to the created linked list.
 */
#define CU_LIST_CREATE(T) (cuListCreate(#T))
#else
#define CU_LIST_CREATE(T) (cuListCreate())
#endif

/**
 * @brief Macro to append data to a typed linked list.
 *
 * @param T The type of elements in the linked list.
 * @param list A pointer to the CuList to which data will be appended.
 * @param data The data to append to the linked list.
 */
#define CU_LIST_APPEND(T, list, data)                                                                                  \
	do                                                                                                                 \
	{                                                                                                                  \
		CU_LIST_TYPE_ASSERT(T, list);                                                                                  \
		T rightData = data;                                                                                            \
		cuListAppend((list), &rightData, sizeof(T));                                                                   \
	} while (CU_FALSE)

/**
 * @brief Macro to retrieve data at a specific index from a typed linked list.
 * @param T The type of elements in the linked list.
 * @param list A pointer to the CuList from which data will be retrieved.
 * @param index The index of the data to retrieve.
 * @return A pointer to the data at the specified index.
 */
#define CU_LIST_GET_AT(T, list, index)                                                                                 \
	((T*)(cuListGetAt((list), (index))));                                                                              \
	do                                                                                                                 \
	{                                                                                                                  \
		CU_LIST_TYPE_ASSERT(T, list);                                                                                  \
	} while (CU_FALSE)

/**
 * @brief Macro to remove a specific node from a typed linked list.
 * @param T The type of elements in the linked list.
 * @param list A pointer to the CuList from which the node will be removed.
 * @param node A pointer to the CuListNode to remove.
 */
#define CU_LIST_REMOVE_NODE(T, list, node)                                                                             \
	do                                                                                                                 \
	{                                                                                                                  \
		CU_LIST_TYPE_ASSERT(T, list);                                                                                  \
		CU_LIST_OWNER_ASSERT(node, list);                                                                              \
		cuListRemoveNode((list), (node));                                                                              \
	} while (CU_FALSE)

/**
 * @brief Macro to insert a new node after a specified node in a typed linked list.
 * @param T The type of elements in the linked list.
 * @param list A pointer to the CuList in which the new node will be inserted.
 * @param node A pointer to the CuListNode after which the new node will be inserted.
 * @param data The data to insert into the new node.
 */
#define CU_LIST_INSERT_NODE_AFTER(T, list, node, data)                                                                 \
	do                                                                                                                 \
	{                                                                                                                  \
		CU_LIST_TYPE_ASSERT(T, list);                                                                                  \
		CU_LIST_OWNER_ASSERT(node, list);                                                                              \
		T rightData = data;                                                                                            \
		cuListInsertNodeAfter((list), (node), &rightData, sizeof(T));                                                  \
	} while (CU_FALSE)

/**
 * @brief Macro to insert a new node before a specified node in a typed linked list.
 * @param T The type of elements in the linked list.
 * @param list A pointer to the CuList in which the new node will be inserted.
 * @param node A pointer to the CuListNode before which the new node will be inserted.
 * @param data The data to insert into the new node.
 */
#define CU_LIST_INSERT_NODE_BEFORE(T, list, node, data)                                                                \
	do                                                                                                                 \
	{                                                                                                                  \
		CU_LIST_TYPE_ASSERT(T, list);                                                                                  \
		CU_LIST_OWNER_ASSERT(node, list);                                                                              \
		T rightData = data;                                                                                            \
		cuListInsertNodeBefore((list), (node), &rightData, sizeof(T));                                                 \
	} while (CU_FALSE)

/**
 * @brief Macro to retrieve data at a specific index from a typed linked list.
 * @param T The type of elements in the linked list.
 * @param list A pointer to the CuList from which data will be retrieved.
 * @param index The index of the data to retrieve.
 * @return A pointer to the data at the specified index.
 */
#define CU_LIST_DELETE(T, list)                                                                                        \
	do                                                                                                                 \
	{                                                                                                                  \
		CU_LIST_TYPE_ASSERT(T, list);                                                                                  \
		cuListDestroy((list));                                                                                         \
	} while (CU_FALSE)
