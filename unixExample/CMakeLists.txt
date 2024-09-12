cmake_minimum_required(VERSION 3.10)

project(ImGuiNotify VERSION 0.0.3 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(SOURCES main.cpp)

set(IMGUI_SOURCES
	${IMGUI_DIR}/imgui/imgui.cpp
	${IMGUI_DIR}/imgui/imgui_demo.cpp
	${IMGUI_DIR}/imgui/imgui_draw.cpp
	${IMGUI_DIR}/imgui/imgui_tables.cpp
	${IMGUI_DIR}/imgui/imgui_widgets.cpp
	${IMGUI_DIR}/imgui/implot.cpp
	${IMGUI_DIR}/imgui/implot_items.cpp
	${IMGUI_DIR}/imgui/implot_demo.cpp
)

set(BACKEND_SOURCES
	${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
	${IMGUI_DIR}/backends/imgui_impl_vulkan.cpp
)

add_executable(test ${SOURCES} ${IMGUI_SOURCES} ${BACKEND_SOURCES})

target_include_directories(test PRIVATE
	${IMGUI_DIR}/imgui
	${IMGUI_DIR}/backends
	${IMGUI_DIR}/src/sources
	${IMGUI_DIR}/src/headers
	${IMGUI_DIR}/fonts
)

find_package(glfw3 REQUIRED)
target_link_libraries(test PRIVATE glfw)

if(UNIX AND NOT APPLE)
	find_package(Vulkan REQUIRED)
	target_link_libraries(test PRIVATE Vulkan::Vulkan)
	target_link_libraries(test PRIVATE pthread dl X11 Xrandr)
endif()

# CMake MacOS Support for unixExample, when installing VulkanSDK choose "Global Source Headers"

if(APPLE)
	find_package(Vulkan REQUIRED)
	target_include_directories(test PRIVATE ${Vulkan_INCLUDE_DIRS})
	target_link_libraries(test PRIVATE ${Vulkan_LIBRARIES})
	# Linking against MoltenVK and Metal frameworks
	find_library(MOLTEN_VK_LIBRARIES NAMES MoltenVK PATHS ${Vulkan_LIBRARY_DIRS})
	find_library(METAL Metal)
	target_link_libraries(test PRIVATE ${MOLTEN_VK_LIBRARIES} ${METAL})
endif()