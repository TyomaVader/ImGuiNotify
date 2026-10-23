cmake_minimum_required(VERSION 3.24)
project(ImGuiNotifyUnixExample VERSION 0.0.3 LANGUAGES CXX)

include(../cmake/Dependencies.cmake)
find_package(glfw3 REQUIRED)
find_package(Vulkan REQUIRED)

add_executable(test main.cpp
    ../examples/common/Demo.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_vulkan.cpp
)
target_include_directories(test PRIVATE
    ../include ../examples/common ${imgui_SOURCE_DIR}/backends
)
target_link_libraries(test PRIVATE imgui_core glfw Vulkan::Vulkan)
