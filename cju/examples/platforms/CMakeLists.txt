file(
    GLOB
    PLATFORM_EXAMPLES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
)

foreach(EXAMPLE ${PLATFORM_EXAMPLES})
    get_filename_component(EXAMPLE_NAME ${EXAMPLE} NAME_WE)

    message(STATUS "Adding platform example: ${EXAMPLE_NAME}")

    add_executable(
        ${EXAMPLE_NAME}
        ${EXAMPLE}
    )

    target_link_libraries(
        ${EXAMPLE_NAME}
        PUBLIC
        CU
    )
endforeach()

