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