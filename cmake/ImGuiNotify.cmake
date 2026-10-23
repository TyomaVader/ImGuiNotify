include_guard(GLOBAL)

add_library(ImGuiNotify INTERFACE)
add_library(ImGuiNotify::ImGuiNotify ALIAS ImGuiNotify)
target_include_directories(ImGuiNotify INTERFACE "${CMAKE_CURRENT_LIST_DIR}/../include")
target_compile_features(ImGuiNotify INTERFACE cxx_std_17)
