cmake_minimum_required(VERSION 3.24)
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    project(ImGuiNotifyDemo LANGUAGES C CXX)
    if(APPLE)
        enable_language(OBJCXX)
    endif()
endif()

include(../../cmake/Dependencies.cmake)
option(IMGUI_NOTIFY_USE_SYSTEM_SDL3 "Use an installed SDL3 CMake package" OFF)
if(IMGUI_NOTIFY_USE_SYSTEM_SDL3)
    find_package(SDL3 3.4.18 CONFIG REQUIRED)
else()
    set(SDL_SHARED OFF CACHE BOOL "Build shared SDL3")
    set(SDL_STATIC ON CACHE BOOL "Build static SDL3")
    set(SDL_TEST_LIBRARY OFF CACHE BOOL "Build SDL3 test library")
    FetchContent_MakeAvailable(SDL3)
endif()

add_executable(imgui_notify_demo
    main.cpp
    ../common/Demo.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp
)
target_include_directories(imgui_notify_demo PRIVATE
    ../common ${imgui_SOURCE_DIR}/backends
)
target_link_libraries(imgui_notify_demo PRIVATE ImGuiNotify::ImGuiNotify imgui_core SDL3::SDL3)

if(APPLE)
    set_source_files_properties(main.cpp PROPERTIES LANGUAGE OBJCXX)
    target_sources(imgui_notify_demo PRIVATE renderer_metal.mm
        ${imgui_SOURCE_DIR}/backends/imgui_impl_metal.mm)
    target_compile_options(imgui_notify_demo PRIVATE $<$<COMPILE_LANGUAGE:OBJCXX>:-fobjc-arc>)
    target_link_libraries(imgui_notify_demo PRIVATE "-framework Metal" "-framework QuartzCore")
elseif(WIN32)
    target_sources(imgui_notify_demo PRIVATE renderer_dx12.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_dx12.cpp)
    target_link_libraries(imgui_notify_demo PRIVATE d3d12 dxgi d3dcompiler dxguid)
    if(TARGET SDL3::SDL3-shared)
        add_custom_command(TARGET imgui_notify_demo POST_BUILD COMMAND ${CMAKE_COMMAND} -E
            copy_if_different $<TARGET_FILE:SDL3::SDL3-shared> $<TARGET_FILE_DIR:imgui_notify_demo>)
    endif()
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    find_package(Vulkan REQUIRED)
    target_sources(imgui_notify_demo PRIVATE renderer_vulkan.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_vulkan.cpp)
    target_link_libraries(imgui_notify_demo PRIVATE Vulkan::Vulkan)
else()
    message(FATAL_ERROR "The demo supports macOS, Windows and Linux")
endif()
