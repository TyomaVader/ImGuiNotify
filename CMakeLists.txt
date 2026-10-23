cmake_minimum_required(VERSION 3.24)
project(ImGuiNotify LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
include(cmake/ImGuiNotify.cmake)

option(IMGUI_NOTIFY_BUILD_EXAMPLES "Build the SDL3 native renderer demo" ${PROJECT_IS_TOP_LEVEL})
if(PROJECT_IS_TOP_LEVEL)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
endif()

if(IMGUI_NOTIFY_BUILD_EXAMPLES)
    enable_language(C)
    if(APPLE)
        enable_language(OBJCXX)
    endif()
    include(cmake/Dependencies.cmake)
    add_subdirectory(examples/sdl3)
endif()
