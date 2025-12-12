cmake_minimum_required(VERSION 3.20)
project(CU)

set(CU_CMAKE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
list(APPEND CMAKE_MODULE_PATH ${CU_CMAKE_PATH})
include(CUSetup)
CU_Setup(${CU_CMAKE_PATH})

option(CU_BUILD_STATIC   "Build CU as a static library" ON)
option(CU_BUILD_SHARED   "Build CU as a shared library" OFF)
option(CU_BUILD_EXAMPLES "Build CU example programs"    OFF)
option(CU_BUILD_TESTS    "Build CU test programs"       OFF)

# Ensure that at least one library is built
if (NOT CU_BUILD_STATIC AND NOT CU_BUILD_SHARED)
    message(FATAL_ERROR "At least one of CU_BUILD_STATIC or CU_BUILD_SHARED must be ON.")
endif()

file(
    GLOB_RECURSE 
    CU_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
)

file(
    GLOB_RECURSE 
    CU_HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/include/cju/*.h"
)

set(CU_ALL_SOURCES
    ${CU_SOURCES}
    ${CU_HEADERS}
)

set(CU_INCLUDE_DIRS
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

set(CU_DEFINITIONS
    ${COMMON_DEFINITIONS}
)

set(CU_TARGET_LINK_OPTIONS)

if (CU_PLATFORM_UNIX)
    list(APPEND CU_TARGET_LINK_OPTIONS -rdynamic)
endif()

set(CU_LINK_LIBS
    ${COMMON_LIBS}
)

if (CU_PLATFORM_UNIX)
    list(APPEND CU_LINK_LIBS dl m pthread)
endif()

set(STATIC_PROJECT_NAME "${PROJECT_NAME}-static")
if (CU_BUILD_STATIC)
    add_library(
        ${STATIC_PROJECT_NAME}
        STATIC
        ${CU_ALL_SOURCES}
    )

    target_include_directories(
        ${STATIC_PROJECT_NAME}
        PUBLIC
        ${CU_INCLUDE_DIRS}
    )

    target_compile_definitions(
        ${STATIC_PROJECT_NAME}
        PUBLIC
        ${CU_DEFINITIONS}
    )
    target_link_libraries(
        ${STATIC_PROJECT_NAME}
        PUBLIC
        ${CU_LINK_LIBS}
    )
    target_link_options(
        ${STATIC_PROJECT_NAME}
        PUBLIC
        ${CU_TARGET_LINK_OPTIONS}
    )
endif()

set(SHARED_PROJECT_NAME "${PROJECT_NAME}-shared")
if (CU_BUILD_SHARED)
    add_library(
        ${SHARED_PROJECT_NAME}
        SHARED
        ${CU_ALL_SOURCES}
    )

    target_include_directories(
        ${SHARED_PROJECT_NAME}
        PUBLIC
        ${CU_INCLUDE_DIRS}
    )

    target_compile_definitions(
        ${SHARED_PROJECT_NAME}
        PUBLIC
        ${CU_DEFINITIONS}
    )
    target_link_libraries(
        ${SHARED_PROJECT_NAME}
        PUBLIC
        ${CU_LINK_LIBS}
    )
    target_link_options(
        ${SHARED_PROJECT_NAME}
        PUBLIC
        ${CU_TARGET_LINK_OPTIONS}
    )
endif()

if (CU_BUILD_STATIC)
    add_library(${PROJECT_NAME} ALIAS ${STATIC_PROJECT_NAME})
else()
    add_library(${PROJECT_NAME} ALIAS ${SHARED_PROJECT_NAME})
endif()

if (CU_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if (CU_BUILD_TESTS)
    set(TEST_PROJECT_NAME "${PROJECT_NAME}-test")
    add_library(
        ${TEST_PROJECT_NAME}
        STATIC
        ${CU_ALL_SOURCES}
    )

    target_include_directories(
        ${TEST_PROJECT_NAME}
        PUBLIC
        ${CU_INCLUDE_DIRS}
    )

    target_compile_definitions(
        ${TEST_PROJECT_NAME}
        PUBLIC
        ${CU_DEFINITIONS}
        CU_TESTING
    )
    target_link_libraries(
        ${TEST_PROJECT_NAME}
        PUBLIC
        ${CU_LINK_LIBS}
    )
    target_link_options(
        ${TEST_PROJECT_NAME}
        PUBLIC
        ${CU_TARGET_LINK_OPTIONS}
    )

    add_subdirectory(tests)
endif()