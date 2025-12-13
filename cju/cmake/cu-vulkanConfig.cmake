if (TARGET cu-vulkan)
    return()
endif()

find_package(Vulkan REQUIRED)

add_library(
    cu-vulkan
    INTERFACE
)

target_include_directories(
    cu-vulkan
    INTERFACE
    ${Vulkan_INCLUDE_DIRS}
)

target_link_libraries(
    cu-vulkan
    INTERFACE 
    Vulkan::Vulkan
)