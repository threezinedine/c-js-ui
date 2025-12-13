#include <common.hpp>

TEST(QueueTest, Create)
{
	CuQueue* pQueue = CU_QUEUE_CREATE(int);
	EXPECT_NE(pQueue, nullptr);

	u32 count = CU_QUEUE_COUNT(int, pQueue);
	EXPECT_EQ(count, 0u);
	CU_QUEUE_DESTROY(int, pQueue);
}

TEST(QueueTest, PushPopFront)
{
	CuQueue* pQueue = CU_QUEUE_CREATE(int);
	EXPECT_NE(pQueue, nullptr);

	CU_QUEUE_PUSH(int, pQueue, 42);
	u32 count = CU_QUEUE_COUNT(int, pQueue);
	EXPECT_EQ(count, 1u);
	int* pFront = CU_QUEUE_FRONT(int, pQueue);
	EXPECT_EQ(*pFront, 42);

	CU_QUEUE_DESTROY(int, pQueue);
}

TEST(QueueTest, MultiplePushPop)
{
	CuQueue* pQueue = CU_QUEUE_CREATE(int);
	EXPECT_NE(pQueue, nullptr);

	for (int i = 0; i < 10; ++i)
	{
		CU_QUEUE_PUSH(int, pQueue, i);
		u32 count = CU_QUEUE_COUNT(int, pQueue);
		EXPECT_EQ(count, static_cast<u32>(i + 1));
		int* pFront = CU_QUEUE_FRONT(int, pQueue);
		EXPECT_EQ(*pFront, 0);
	}

	for (int i = 9; i >= 0; --i)
	{
		CU_QUEUE_POP(int, pQueue);
		u32 count = CU_QUEUE_COUNT(int, pQueue);
		EXPECT_EQ(count, static_cast<u32>(i));

		if (i > 0)
		{
			int* pFront = CU_QUEUE_FRONT(int, pQueue);
			EXPECT_EQ(*pFront, 10 - i);
		}
	}

	CU_QUEUE_DESTROY(int, pQueue);
}

TEST(QueueTest, PopEmptyQueue)
{
	CuQueue* pQueue = CU_QUEUE_CREATE(int);
	EXPECT_NE(pQueue, nullptr);

	EXPECT_EXIT(CU_QUEUE_POP(int, pQueue);, ::testing::ExitedWithCode(CU_EXCEPTION_CODE_INDEX_OUT_OF_BOUNDS), "");

	CU_QUEUE_DESTROY(int, pQueue);
}

TEST(QueueTest, FrontEmptyQueue)
{
	CuQueue* pQueue = CU_QUEUE_CREATE(int);
	EXPECT_NE(pQueue, nullptr);

	EXPECT_EXIT(CU_QUEUE_FRONT(int, pQueue);, ::testing::ExitedWithCode(CU_EXCEPTION_CODE_INDEX_OUT_OF_BOUNDS), "");

	CU_QUEUE_DESTROY(int, pQueue);
}

#if CU_DEBUG
TEST(QueueTest, PushDifferentTypes)
{
	CuQueue* pFloatQueue = CU_QUEUE_CREATE(float);
	EXPECT_NE(pFloatQueue, nullptr);

	CU_QUEUE_PUSH(float, pFloatQueue, 3.14f);

	EXPECT_EXIT(CU_QUEUE_COUNT(int, pFloatQueue);, ::testing::ExitedWithCode(CU_EXCEPTION_CODE_INVALID_TYPE), "");

	CU_QUEUE_DESTROY(float, pFloatQueue);
}
#endif // CU_DEBUG