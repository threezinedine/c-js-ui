#include "common.hpp"

TEST(ListTest, Create)
{
	CuList* pList = CU_LIST_CREATE(int);

	EXPECT_EQ(pList->pHead, nullptr);
	EXPECT_EQ(pList->pTail, nullptr);
	EXPECT_EQ(pList->count, 0);

	CU_LIST_DELETE(int, pList);
}

TEST(ListTest, AppendAndGetAt)
{
	CuList* pList = CU_LIST_CREATE(int);

	int values[] = {10, 20, 30, 40, 50};
	for (int i = 0; i < 5; ++i)
	{
		CU_LIST_APPEND(int, pList, values[i]);
	}

	EXPECT_EQ(pList->count, 5);

	for (int i = 0; i < 5; ++i)
	{
		int* pValue = CU_LIST_GET_AT(int, pList, i);
		EXPECT_NE(pValue, nullptr);
		EXPECT_EQ(*pValue, values[i]);
	}

	CU_LIST_DELETE(int, pList);
}

TEST(ListTest, RemoveElement)
{
	CuList* pList = CU_LIST_CREATE(int);

	int values[] = {10, 20, 30, 40, 50};

	for (int i = 0; i < 5; ++i)
	{
		CU_LIST_APPEND(int, pList, values[i]);
	}

	EXPECT_EQ(pList->count, 5);

	CuListNode* pCurrent = pList->pHead;

	u32 index = 0;

	while (index != 3)
	{
		pCurrent = pCurrent->pNext;
		index++;
	}

	CU_LIST_REMOVE_NODE(int, pList, pCurrent);

	EXPECT_EQ(pList->count, 4);

	int expectedValuesAfterRemoval[] = {10, 20, 30, 50};

	for (int i = 0; i < 4; ++i)
	{
		int* pValue = CU_LIST_GET_AT(int, pList, i);
		EXPECT_NE(pValue, nullptr);
		EXPECT_EQ(*pValue, expectedValuesAfterRemoval[i]);
	}

	CU_LIST_DELETE(int, pList);
}

TEST(ListTest, Clear)
{
	CuList* pList = CU_LIST_CREATE(int);

	int values[] = {10, 20, 30, 40, 50};

	for (int i = 0; i < 5; ++i)
	{
		CU_LIST_APPEND(int, pList, values[i]);
	}

	EXPECT_EQ(pList->count, 5);

	cuListClear(pList);

	EXPECT_EQ(pList->count, 0);
	EXPECT_EQ(pList->pHead, nullptr);
	EXPECT_EQ(pList->pTail, nullptr);

	CU_LIST_DELETE(int, pList);
}

TEST(ListTest, GetAtOutOfBounds)
{
	CuList* pList = CU_LIST_CREATE(int);

	int values[] = {10, 20, 30};

	for (int i = 0; i < 3; ++i)
	{
		CU_LIST_APPEND(int, pList, values[i]);
	}

	EXPECT_EQ(pList->count, 3);

	EXPECT_EXIT(CU_LIST_GET_AT(int, pList, 5), ::testing::ExitedWithCode(CU_EXCEPTION_CODE_INDEX_OUT_OF_BOUNDS), "");

	CU_LIST_DELETE(int, pList);
}

TEST(ListTest, DoSomethingWithDifferentType)
{
	CuList* pList = CU_LIST_CREATE(double);

	CU_LIST_APPEND(double, pList, 3.14);

	EXPECT_EXIT(CU_LIST_GET_AT(int, pList, 0), ::testing::ExitedWithCode(CU_EXCEPTION_CODE_INVALID_TYPE), "");

	CU_LIST_DELETE(double, pList);
}

TEST(ListTest, InsertAfterNode)
{
	CuList* pList = CU_LIST_CREATE(int);

	int values[] = {10, 20, 30};

	for (int i = 0; i < 3; ++i)
	{
		CU_LIST_APPEND(int, pList, values[i]);
	}

	EXPECT_EQ(pList->count, 3);

	// Insert the new node between the first and second nodes
	CuListNode* pSecondNode = pList->pHead->pNext;

	CU_LIST_INSERT_NODE_AFTER(int, pList, pSecondNode, 25);

	int expectedValuesAfterInsertion[] = {10, 20, 25, 30};

	for (int i = 0; i < 4; ++i)
	{
		int* pValue = CU_LIST_GET_AT(int, pList, i);
		EXPECT_NE(pValue, nullptr);
		EXPECT_EQ(*pValue, expectedValuesAfterInsertion[i]);
	}

	// Clean up
	CU_LIST_DELETE(int, pList);
}

TEST(ListTest, InsertBeforeNode)
{
	CuList* pList = CU_LIST_CREATE(int);

	int values[] = {10, 20, 30};

	for (int i = 0; i < 3; ++i)
	{
		CU_LIST_APPEND(int, pList, values[i]);
	}

	EXPECT_EQ(pList->count, 3);

	CU_LIST_INSERT_NODE_BEFORE(int, pList, pList->pHead, 25);

	int expectedValuesAfterInsertion[] = {25, 10, 20, 30};

	for (int i = 0; i < 4; ++i)
	{
		int* pValue = CU_LIST_GET_AT(int, pList, i);
		EXPECT_NE(pValue, nullptr);
		EXPECT_EQ(*pValue, expectedValuesAfterInsertion[i]);
	}

	// Clean up
	CU_LIST_DELETE(int, pList);
}