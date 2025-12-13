project(CJUTests)


file(
    GLOB_RECURSE 
    CU_TEST_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

file(
    GLOB_RECURSE 
    CU_TEST_HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
)

add_executable(
    ${PROJECT_NAME}
    ${CU_TEST_SOURCES}
    ${CU_TEST_HEADERS}
)

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

if (CU_DEBUG)
    target_compile_definitions(
        ${PROJECT_NAME}
        PRIVATE
        CU_DEBUG
    )
endif()

CU_FindPackage(cu-googletest "Dependencies")

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
    CU-test
    cu-googletest
)