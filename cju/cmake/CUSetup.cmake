macro(CU_FindPackage PACKAGE_NAME FOLDER_NAME)
    set(${PACKAGE_NAME}_DIR "${CU_CMAKE_MODULE_PATH}")

    set(CMAKE_FOLDER ${FOLDER_NAME})
    find_package(${PACKAGE_NAME} REQUIRED)
    unset(CMAKE_FOLDER)
endmacro()

macro(CU_Option OPTION_NAME OPTION_DEFAULT)
    option(${OPTION_NAME} "CU Option: ${OPTION_NAME}" ${OPTION_DEFAULT})

    list(APPEND CU_OPTIONS ${OPTION_NAME})
endmacro()

macro(CU_PlatformDetection)
    set(CU_PLATFORM_WINDOWS FALSE)
    set(CU_PLATFORM_UNIX FALSE)
    set(CU_PLATFORM_WEB FALSE)

    if (WIN32)
        set(CU_PLATFORM_WINDOWS TRUE)
        list(APPEND COMMON_DEFINITIONS CU_PLATFORM_WINDOWS=1)
    elseif(UNIX)
        set(CU_PLATFORM_UNIX TRUE)
        list(APPEND COMMON_DEFINITIONS CU_PLATFORM_UNIX=1)
    elseif(EMSCRIPTEN)
        set(CU_PLATFORM_WEB TRUE)
        list(APPEND COMMON_DEFINITIONS CU_PLATFORM_WEB=1)
    else()
        message(FATAL_ERROR "Unsupported platform")
    endif()

    include(TestBigEndian)
    test_big_endian(IS_BIG_ENDIAN)

    if (IS_BIG_ENDIAN)
        list(APPEND COMMON_DEFINITIONS CU_PLATFORM_BIG_ENDIAN=1)
    else()
        list(APPEND COMMON_DEFINITIONS CU_PLATFORM_LITTLE_ENDIAN=1)
    endif()
endmacro()


macro(CU_Setup)
    set(COMMON_DEFINITIONS "")
    CU_PlatformDetection()

    option(CMAKE_BUILD_TYPE "Build type" Debug)

    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        list(APPEND COMMON_DEFINITIONS CU_DEBUG=1)
    else()
        list(APPEND COMMON_DEFINITIONS CU_RELEASE=1)
    endif()

    foreach(OPTION ${CU_OPTIONS})
        if(${OPTION})
            list(APPEND COMMON_DEFINITIONS ${OPTION}=1)
        else()
            list(APPEND COMMON_DEFINITIONS ${OPTION}=0)
        endif()
    endforeach()

    message(STATUS "Definitions:")
    foreach(DEF ${COMMON_DEFINITIONS})
        message(STATUS "  ${DEF}")
    endforeach()
endmacro()
