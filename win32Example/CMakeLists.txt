cmake_minimum_required(VERSION 3.24)
project(ImGuiNotifyWindowsExample VERSION 0.0.3 LANGUAGES CXX)

include(../cmake/Dependencies.cmake)

add_executable(test main.cpp
    ../examples/common/Demo.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_win32.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_dx12.cpp
)
target_include_directories(test PRIVATE
    ../include ../examples/common ${imgui_SOURCE_DIR}/backends
)
target_link_libraries(test PRIVATE imgui_core d3d12 dxgi d3dcompiler)
