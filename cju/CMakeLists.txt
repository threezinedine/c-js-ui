cmake_minimum_required(VERSION 3.20)
project(CU)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(CUSetup)
CU_Setup()

option(CU_BUILD_STATIC   "Build CU as a static library" ON)
option(CU_BUILD_SHARED   "Build CU as a shared library" OFF)
option(CU_BUILD_EXAMPLES "Build CU example programs"    OFF)

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
endif()

if (CU_BUILD_STATIC)
    add_library(${PROJECT_NAME} ALIAS ${STATIC_PROJECT_NAME})
else()
    add_library(${PROJECT_NAME} ALIAS ${SHARED_PROJECT_NAME})
endif()

if (CU_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()