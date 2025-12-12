#include "common.hpp"

TEST(ArrayTest, Initialization)
{
	CuArray* pArray = CU_ARRAY_INIT(int, 10);

	EXPECT_EQ(pArray->count, 0);
	EXPECT_EQ(pArray->capacity, 10);

	CU_ARRAY_FREE(int, pArray);
}

TEST(ArrayTest, ResizeWithSmallerCapacity)
{
	CuArray* pArray = CU_ARRAY_INIT(int, 10);

	CU_ARRAY_RESIZE(int, pArray, 5);

	EXPECT_EQ(pArray->count, 0);
	EXPECT_EQ(pArray->capacity, 10);

	CU_ARRAY_FREE(int, pArray);
}

TEST(ArrayTest, ResizeWithLargerCapacity)
{
	CuArray* pArray = CU_ARRAY_INIT(int, 10);

	CU_ARRAY_RESIZE(int, pArray, 20);

	EXPECT_EQ(pArray->count, 0);
	EXPECT_EQ(pArray->capacity, 20);

	CU_ARRAY_FREE(int, pArray);
}

TEST(ArrayTest, PushBack)
{
	CuArray* pArray = CU_ARRAY_INIT(int, 2);

	CU_ARRAY_PUSH_BACK(int, pArray, 1);

	CU_ARRAY_FREE(int, pArray);
}

TEST(ArrayTest, GetIndex)
{
	CuArray* pArray = CU_ARRAY_INIT(int, 2);

	CU_ARRAY_PUSH_BACK(int, pArray, 42);
	int* value = CU_ARRAY_GET(int, pArray, 0);
	EXPECT_EQ(*value, 42);

	CU_ARRAY_FREE(int, pArray);
}

TEST(ArrayTest, GetIndexOutOfBounds)
{
	CuArray* pArray = CU_ARRAY_INIT(int, 2);

	CU_ARRAY_PUSH_BACK(int, pArray, 42);

	EXPECT_EXIT(
		{ CU_ARRAY_GET(int, pArray, 1); }, ::testing::ExitedWithCode(CU_EXCEPTION_CODE_INDEX_OUT_OF_BOUNDS), "");

	CU_ARRAY_FREE(int, pArray);
}

TEST(ArrayTest, DoSomethingWithDifferentType)
{
	CuArray* pArray = CU_ARRAY_INIT(double, 3);

	CU_ARRAY_PUSH_BACK(double, pArray, 3.14);
	EXPECT_EXIT(CU_ARRAY_GET(int, pArray, 0), ::testing::ExitedWithCode(CU_EXCEPTION_CODE_INVALID_TYPE), "");

	CU_ARRAY_FREE(double, pArray);
}