file(
    GLOB
    MODULE_EXAMPLES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
)

foreach(EXAMPLE ${MODULE_EXAMPLES})
    get_filename_component(EXAMPLE_NAME ${EXAMPLE} NAME_WE)

    message(STATUS "Adding module example: ${EXAMPLE_NAME}")

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


