if (TARGET cu-googletest)
    return()
endif()

include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG main
)

FetchContent_MakeAvailable(googletest)

add_library(cu-googletest INTERFACE)

target_link_libraries(cu-googletest INTERFACE gtest gtest_main gmock gmock_main)