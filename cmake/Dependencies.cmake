include_guard(GLOBAL)
include(FetchContent)
include("${CMAKE_CURRENT_LIST_DIR}/ImGuiNotify.cmake")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Standalone examples/tests only. Library consumers supply their own Dear ImGui.
FetchContent_Declare(imgui
    URL https://codeload.github.com/ocornut/imgui/tar.gz/refs/tags/v1.93.0-docking
    URL_HASH SHA256=746c265540bd3aa109570f29492ef4d8f5b421dd9fd0049c35ca35f21496f1a5
)

# Populated only by the SDL3 demo; headless checks do not build SDL3.
FetchContent_Declare(SDL3
    URL https://codeload.github.com/libsdl-org/SDL/tar.gz/refs/tags/release-3.4.18
    URL_HASH SHA256=c4b08b950bd29d83caae0ab8d884298d27046a9d30700fb42841c50992665c90
)

FetchContent_MakeAvailable(imgui)
add_library(imgui_core STATIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
)
target_include_directories(imgui_core PUBLIC ${imgui_SOURCE_DIR})
target_compile_features(imgui_core PUBLIC cxx_std_17)
