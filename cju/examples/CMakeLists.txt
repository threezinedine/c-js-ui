file(
    GLOB
    EXAMPLES
    "${CMAKE_CURRENT_SOURCE_DIR}/*"
)

foreach(EXAMPLE ${EXAMPLES})
    if (IS_DIRECTORY ${EXAMPLE})
        add_subdirectory(${EXAMPLE})
    endif() 
endforeach()
