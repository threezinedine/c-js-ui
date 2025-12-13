#include "common.hpp"

TEST(ArrayTest, Initialization)
{
	CuArray* pArray = CU_ARRAY_CREATE(int, 10);

	EXPECT_EQ(pArray->count, 0);
	EXPECT_EQ(pArray->capacity, 10);

	CU_ARRAY_DELETE(int, pArray);
}

TEST(ArrayTest, ResizeWithSmallerCapacity)
{
	CuArray* pArray = CU_ARRAY_CREATE(int, 10);

	CU_ARRAY_RESIZE(int, pArray, 5);

	EXPECT_EQ(pArray->count, 0);
	EXPECT_EQ(pArray->capacity, 10);

	CU_ARRAY_DELETE(int, pArray);
}

TEST(ArrayTest, ResizeWithLargerCapacity)
{
	CuArray* pArray = CU_ARRAY_CREATE(int, 10);

	CU_ARRAY_RESIZE(int, pArray, 20);

	EXPECT_EQ(pArray->count, 0);
	EXPECT_EQ(pArray->capacity, 20);

	CU_ARRAY_DELETE(int, pArray);
}

TEST(ArrayTest, PushBack)
{
	CuArray* pArray = CU_ARRAY_CREATE(int, 2);

	CU_ARRAY_PUSH_BACK(int, pArray, 1);

	CU_ARRAY_DELETE(int, pArray);
}

TEST(ArrayTest, GetIndex)
{
	CuArray* pArray = CU_ARRAY_CREATE(int, 2);

	CU_ARRAY_PUSH_BACK(int, pArray, 42);
	int* value = CU_ARRAY_GET(int, pArray, 0);
	EXPECT_EQ(*value, 42);

	CU_ARRAY_DELETE(int, pArray);
}

TEST(ArrayTest, GetIndexOutOfBounds)
{
	CuArray* pArray = CU_ARRAY_CREATE(int, 2);

	CU_ARRAY_PUSH_BACK(int, pArray, 42);

	EXPECT_EXIT(
		{ CU_ARRAY_GET(int, pArray, 1); }, ::testing::ExitedWithCode(CU_EXCEPTION_CODE_INDEX_OUT_OF_BOUNDS), "");

	CU_ARRAY_DELETE(int, pArray);
}

TEST(ArrayTest, DoSomethingWithDifferentType)
{
	CuArray* pArray = CU_ARRAY_CREATE(double, 3);

	CU_ARRAY_PUSH_BACK(double, pArray, 3.14);
	EXPECT_EXIT(CU_ARRAY_GET(int, pArray, 0), ::testing::ExitedWithCode(CU_EXCEPTION_CODE_INVALID_TYPE), "");

	CU_ARRAY_DELETE(double, pArray);
}

TEST(ArrayTest, PushIndexOutOfCapacity)
{
	CuArray* pArray = CU_ARRAY_CREATE(int, 2);

	CU_ARRAY_PUSH_BACK(int, pArray, 1);
	CU_ARRAY_PUSH_BACK(int, pArray, 2);
	CU_ARRAY_PUSH_BACK(int, pArray, 3); // This should trigger a resize

	EXPECT_EQ(pArray->count, 3);
	EXPECT_GE(pArray->capacity, 4);

	int* value = CU_ARRAY_GET(int, pArray, 2);
	EXPECT_EQ(*value, 3);

	CU_ARRAY_DELETE(int, pArray);
}

TEST(ArrayTest, Clear)
{
	CuArray* pArray = CU_ARRAY_CREATE(int, 2);

	CU_ARRAY_PUSH_BACK(int, pArray, 1);
	CU_ARRAY_PUSH_BACK(int, pArray, 2);
	CU_ARRAY_PUSH_BACK(int, pArray, 3);

	EXPECT_EQ(pArray->count, 3);

	CU_ARRAY_CLEAR(int, pArray);

	EXPECT_EQ(pArray->count, 0);
	EXPECT_EQ(pArray->capacity, 4);

	CU_ARRAY_DELETE(int, pArray);
}

TEST(ArrayTest, ArrayWithPointer)
{
	CuArray* pArray = CU_ARRAY_CREATE(int*, 2);

	int a = 10;
	int b = 20;

	CU_ARRAY_PUSH_BACK(int*, pArray, &a);
	CU_ARRAY_PUSH_BACK(int*, pArray, &b);

	int** ptrA = CU_ARRAY_GET(int*, pArray, 0);
	int** ptrB = CU_ARRAY_GET(int*, pArray, 1);

	EXPECT_EQ(**ptrA, 10);
	EXPECT_EQ(**ptrB, 20);

	CU_ARRAY_DELETE(int*, pArray);
}

TEST(ArrayTest, InsertAtTheBegging)
{
	CuArray* pArray = CU_ARRAY_CREATE(int, 2);

	CU_ARRAY_PUSH_BACK(int, pArray, 2);
	CU_ARRAY_PUSH_BACK(int, pArray, 3);

	CU_ARRAY_INSERT(int, pArray, 0, 1);

	EXPECT_EQ(pArray->count, 3);

	int* firstValue = CU_ARRAY_GET(int, pArray, 0);
	EXPECT_EQ(*firstValue, 1);

	CU_ARRAY_DELETE(int, pArray);
}

TEST(ArrayTest, InsertAtTheMiddle)
{
	CuArray* pArray = CU_ARRAY_CREATE(int, 2);

	CU_ARRAY_PUSH_BACK(int, pArray, 1);
	CU_ARRAY_PUSH_BACK(int, pArray, 3);

	CU_ARRAY_INSERT(int, pArray, 1, 2);

	EXPECT_EQ(pArray->count, 3);

	int* secondValue = CU_ARRAY_GET(int, pArray, 1);
	EXPECT_EQ(*secondValue, 2);

	CU_ARRAY_DELETE(int, pArray);
}

TEST(ArrayTest, InsertAtTheEnd)
{
	CuArray* pArray = CU_ARRAY_CREATE(int, 2);

	CU_ARRAY_PUSH_BACK(int, pArray, 1);
	CU_ARRAY_PUSH_BACK(int, pArray, 2);

	CU_ARRAY_INSERT(int, pArray, 2, 3);

	EXPECT_EQ(pArray->count, 3);

	int* thirdValue = CU_ARRAY_GET(int, pArray, 2);
	EXPECT_EQ(*thirdValue, 3);

	CU_ARRAY_DELETE(int, pArray);
}

TEST(ArrayTest, InsertAtOutOfBoundsIndex)
{
	CuArray* pArray = CU_ARRAY_CREATE(int, 2);

	CU_ARRAY_PUSH_BACK(int, pArray, 1);
	CU_ARRAY_PUSH_BACK(int, pArray, 2);

	EXPECT_EXIT(
		{ CU_ARRAY_INSERT(int, pArray, 3, 3); }, ::testing::ExitedWithCode(CU_EXCEPTION_CODE_INDEX_OUT_OF_BOUNDS), "");

	CU_ARRAY_DELETE(int, pArray);
}

TEST(ArrayTest, RemoveAtBegging)
{
	CuArray* pArray = CU_ARRAY_CREATE(int, 2);

	CU_ARRAY_PUSH_BACK(int, pArray, 1);
	CU_ARRAY_PUSH_BACK(int, pArray, 2);
	CU_ARRAY_PUSH_BACK(int, pArray, 3);

	CU_ARRAY_REMOVE_AT(int, pArray, 0);

	EXPECT_EQ(pArray->count, 2);

	int* secondValue = CU_ARRAY_GET(int, pArray, 1);
	EXPECT_EQ(*secondValue, 3);

	int* firstValue = CU_ARRAY_GET(int, pArray, 0);
	EXPECT_EQ(*firstValue, 2);

	CU_ARRAY_DELETE(int, pArray);
}

TEST(ArrayTest, RemoveAtMiddle)
{
	CuArray* pArray = CU_ARRAY_CREATE(int, 2);

	CU_ARRAY_PUSH_BACK(int, pArray, 1);
	CU_ARRAY_PUSH_BACK(int, pArray, 2);
	CU_ARRAY_PUSH_BACK(int, pArray, 3);

	CU_ARRAY_REMOVE_AT(int, pArray, 1);

	EXPECT_EQ(pArray->count, 2);

	int* secondValue = CU_ARRAY_GET(int, pArray, 1);
	EXPECT_EQ(*secondValue, 3);

	CU_ARRAY_DELETE(int, pArray);
}

TEST(ArrayTest, RemoveAtEnd)
{
	CuArray* pArray = CU_ARRAY_CREATE(int, 2);

	CU_ARRAY_PUSH_BACK(int, pArray, 1);
	CU_ARRAY_PUSH_BACK(int, pArray, 2);
	CU_ARRAY_PUSH_BACK(int, pArray, 3);

	CU_ARRAY_REMOVE_AT(int, pArray, 2);

	EXPECT_EQ(pArray->count, 2);

	int* secondValue = CU_ARRAY_GET(int, pArray, 1);
	EXPECT_EQ(*secondValue, 2);

	int* firstValue = CU_ARRAY_GET(int, pArray, 0);
	EXPECT_EQ(*firstValue, 1);

	CU_ARRAY_DELETE(int, pArray);
}