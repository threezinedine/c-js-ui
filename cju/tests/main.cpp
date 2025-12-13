#include "common.hpp"

int main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	auto result = RUN_ALL_TESTS();

	CU_PLATFORM_API(cuMemoryAssertNoLeaks)();

	return result;
}