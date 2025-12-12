cmake_minimum_required(VERSION 3.20)
project(CJU)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(CJUSetup)
CJU_Setup()

option(CJU_BUILD_STATIC   "Build CJU as a static library" ON)
option(CJU_BUILD_SHARED   "Build CJU as a shared library" OFF)
option(CJU_BUILD_EXAMPLES "Build CJU example programs"    OFF)

# Ensure that at least one library is built
if (NOT CJU_BUILD_STATIC AND NOT CJU_BUILD_SHARED)
    message(FATAL_ERROR "At least one of CJU_BUILD_STATIC or CJU_BUILD_SHARED must be ON.")
endif()

file(
    GLOB_RECURSE 
    CJU_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
)

file(
    GLOB_RECURSE 
    CJU_HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/include/cju/*.h"
)

set(CJU_ALL_SOURCES
    ${CJU_SOURCES}
    ${CJU_HEADERS}
)

set(CJU_INCLUDE_DIRS
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

set(CJU_DEFINITIONS
    ${COMMON_DEFINITIONS}
)

set(STATIC_PROJECT_NAME "${PROJECT_NAME}-static")
if (CJU_BUILD_STATIC)
    add_library(
        ${STATIC_PROJECT_NAME}
        STATIC
        ${CJU_ALL_SOURCES}
    )

    target_include_directories(
        ${STATIC_PROJECT_NAME}
        PUBLIC
        ${CJU_INCLUDE_DIRS}
    )

    target_compile_definitions(
        ${STATIC_PROJECT_NAME}
        PUBLIC
        ${CJU_DEFINITIONS}
    )
endif()

set(SHARED_PROJECT_NAME "${PROJECT_NAME}-shared")
if (CJU_BUILD_SHARED)
    add_library(
        ${SHARED_PROJECT_NAME}
        SHARED
        ${CJU_ALL_SOURCES}
    )

    target_include_directories(
        ${SHARED_PROJECT_NAME}
        PUBLIC
        ${CJU_INCLUDE_DIRS}
    )

    target_compile_definitions(
        ${SHARED_PROJECT_NAME}
        PUBLIC
        ${CJU_DEFINITIONS}
    )
endif()

if (CJU_BUILD_STATIC)
    add_library(${PROJECT_NAME} ALIAS ${STATIC_PROJECT_NAME})
else()
    add_library(${PROJECT_NAME} ALIAS ${SHARED_PROJECT_NAME})
endif()

if (CJU_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()