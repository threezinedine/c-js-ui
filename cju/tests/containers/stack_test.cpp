#include <common.hpp>

TEST(StackTest, Create)
{
	CuStack* pStack = CU_STACK_CREATE(int);
	EXPECT_NE(pStack, nullptr);

	u32 count = CU_STACK_COUNT(int, pStack);
	EXPECT_EQ(count, 0u);
	CU_STACK_DESTROY(int, pStack);
}

TEST(StackTest, PushPopTop)
{
	CuStack* pStack = CU_STACK_CREATE(int);
	EXPECT_NE(pStack, nullptr);

	CU_STACK_PUSH(int, pStack, 42);
	u32 count = CU_STACK_COUNT(int, pStack);
	EXPECT_EQ(count, 1u);
	int* pTop = CU_STACK_TOP(int, pStack);
	EXPECT_EQ(*pTop, 42);

	CU_STACK_DESTROY(int, pStack);
}

TEST(StackTest, MultiplePushPop)
{
	CuStack* pStack = CU_STACK_CREATE(int);
	EXPECT_NE(pStack, nullptr);

	for (int i = 0; i < 10; ++i)
	{
		CU_STACK_PUSH(int, pStack, i);
		u32 count = CU_STACK_COUNT(int, pStack);
		EXPECT_EQ(count, static_cast<u32>(i + 1));
		int* pTop = CU_STACK_TOP(int, pStack);
		EXPECT_EQ(*pTop, i);
	}

	for (int i = 9; i >= 0; --i)
	{
		int* pTop = CU_STACK_TOP(int, pStack);
		EXPECT_EQ(*pTop, i);
		CU_STACK_POP(int, pStack);
		u32 count = CU_STACK_COUNT(int, pStack);
		EXPECT_EQ(count, static_cast<u32>(i));

		if (i > 0)
		{
			int* pTop = CU_STACK_TOP(int, pStack);
			EXPECT_EQ(*pTop, i - 1);
		}
	}

	CU_STACK_DESTROY(int, pStack);
}

TEST(StackTest, PopEmptyStack)
{
	CuStack* pStack = CU_STACK_CREATE(int);
	EXPECT_NE(pStack, nullptr);

	EXPECT_EXIT(CU_STACK_POP(int, pStack);, ::testing::ExitedWithCode(CU_EXCEPTION_CODE_INDEX_OUT_OF_BOUNDS), "");

	CU_STACK_DESTROY(int, pStack);
}

#if CU_DEBUG
TEST(StackTest, PushDifferentTypes)
{
	CuStack* pFloatStack = CU_STACK_CREATE(float);
	EXPECT_NE(pFloatStack, nullptr);

	CU_STACK_PUSH(float, pFloatStack, 3.14f);

	EXPECT_EXIT(CU_STACK_COUNT(int, pFloatStack);, ::testing::ExitedWithCode(CU_EXCEPTION_CODE_INVALID_TYPE), "");

	CU_STACK_DESTROY(float, pFloatStack);
}
#endif // CU_DEBUG