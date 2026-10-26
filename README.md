#  ImGuiNotify

Is a header-only wrapper made to create notifications with [Dear ImGui](https://github.com/ocornut/imgui). Fork of [imgui-notify](https://github.com/patrickcjk/imgui-notify) by [patrickcjk](https://github.com/patrickcjk).

> [!IMPORTANT]
> Requires [Font Awesome 6](https://fontawesome.com/) for icons ([Setup example](https://github.com/juliettef/IconFontCppHeaders)).

> [!IMPORTANT]
> Requires [C++17](https://en.cppreference.com/w/cpp/17) or later.

> [!IMPORTANT]
> Requires Dear ImGui docking branch with multi-viewport enabled for rendering outside the main viewport

## Usage

Add `include/` to your compiler's include paths and link your application's Dear ImGui and backends.

For the font setup below, also add `examples/assets/` to your include paths, or load your own Font Awesome 6 font.

### Include

```c++
#include "ImGuiNotify.hpp"
#include "IconsFontAwesome6.h"
#include "fa-solid-900.h"
```

### Initialisation (after impl call: ImGui_ImplDX12_Init, ImGui_ImplVulkan_Init, etc.)

```c++
ImFontConfig defaultConfig;
defaultConfig.SizePixels = 13.0f;
io.Fonts->AddFontDefault(&defaultConfig);

float baseFontSize = 16.0f;
float iconFontSize = baseFontSize * 2.0f / 3.0f; // FontAwesome fonts need to have their sizes reduced by 2.0f/3.0f in order to align correctly

static constexpr ImWchar iconsRanges[] = {ICON_MIN_FA, ICON_MAX_16_FA, 0};
ImFontConfig iconsConfig;
iconsConfig.MergeMode = true;
iconsConfig.PixelSnapH = true;
iconsConfig.GlyphMinAdvanceX = iconFontSize;
io.Fonts->AddFontFromMemoryCompressedTTF(fa_solid_900_compressed_data, fa_solid_900_compressed_size, iconFontSize, &iconsConfig, iconsRanges);
```

### Create notifications

- Success

```c++
ImGui::InsertNotification({ImGuiToastType::Success, 3000, "That is a success! %s", "(Format here)"});
```

![success](https://github.com/TyomaVader/ImGuiNotify/assets/67346255/1d4768d0-a20a-45c3-a939-c6aeb21bbfa8)

- Warning

```c++
ImGui::InsertNotification({ImGuiToastType::Warning, 3000, "Hello World! This is a warning! %d", 0x1337});
```

![warning](https://github.com/TyomaVader/ImGuiNotify/assets/67346255/99f84318-b2d9-4e7e-a584-68bef49cf095)

- Error

```c++
ImGui::InsertNotification({ImGuiToastType::Error, 3000, "Hello World! This is an error! 0x%X", 0xDEADBEEF});
```

![error](https://github.com/TyomaVader/ImGuiNotify/assets/67346255/d777ecba-bfc1-42a6-ac6e-2601396045eb)

- Info

```c++
ImGui::InsertNotification({ImGuiToastType::Info, 3000, "Hello World! This is an info!"});
```

![info](https://github.com/TyomaVader/ImGuiNotify/assets/67346255/cd3660cc-d264-43dd-b994-09c084873c20)

- Long info

```c++
ImGui::InsertNotification({ImGuiToastType::Info, 3000, "Hi, I'm a long notification. I'm here to show you that you can write a lot of text in me. I'm also here to show you that I can wrap text, so you don't have to worry about that."});
```

![longInfo](https://github.com/TyomaVader/ImGuiNotify/assets/67346255/52819d26-5b2b-49e9-abca-293d70927b77)

- Error with button

```c++
ImGui::InsertNotification({ImGuiToastType::Error, 3000, "Click me!", [](){ImGui::InsertNotification({ImGuiToastType::Success, 3000, "Thanks for clicking!"});}, "Notification content"});
```

![withButton](https://github.com/TyomaVader/ImGuiNotify/assets/67346255/a9e85d75-9a10-4b31-935f-bcc7785d196d)

- Now using a custom title...

```c++
ImGuiToast toast(ImGuiToastType::Success, 3000); // <-- content can also be passed here as above
toast.setTitle("This is a %s title", "wonderful");
toast.setContent("Lorem ipsum dolor sit amet");
ImGui::InsertNotification(toast);
```

![customTitle](https://github.com/TyomaVader/ImGuiNotify/assets/67346255/b77a6f45-1cb4-4a22-9ff9-41736e56afe4)

### Rendering

```c++
// Notifications style setup
ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.f); // Disable round borders
ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f); // Disable borders

// Notifications color setup
ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.10f, 0.10f, 0.10f, 1.00f)); // Background color

// Main rendering function
ImGui::RenderNotifications();

//——————————————————————————————— WARNING ———————————————————————————————
// Argument MUST match the amount of ImGui::PushStyleVar() calls 
ImGui::PopStyleVar(2);
// Argument MUST match the amount of ImGui::PushStyleColor() calls 
ImGui::PopStyleColor(1);
```

## Showcase

> [!NOTE]
> The following preview uses an Embrace The Darkness theme by [@janekb04](https://github.com/janekb04). The shared theme is in [Demo.cpp](examples/common/Demo.cpp).

https://github.com/TyomaVader/ImGuiNotify/assets/67346255/c4d9ee3c-5725-4d8d-89ee-670d9ecd3378

## Build the demo

Requires CMake 3.24 or later and a C++17 compiler. CMake downloads Dear ImGui and SDL3 automatically.
The shared [SDL3 demo](examples/sdl3) uses DirectX12 on Windows, Vulkan on Linux, and Metal on macOS.

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
./build/imgui_notify_demo
```

On Windows, run `build/Release/imgui_notify_demo.exe`. macOS needs the Xcode Command Line Tools. Linux needs Vulkan headers and loader plus SDL's build dependencies.

To use an installed SDL3 development package, add `-DIMGUI_NOTIFY_USE_SYSTEM_SDL3=ON` when configuring.

Applications can use the header library through CMake:

```cmake
add_subdirectory(path/to/ImGuiNotify)
target_link_libraries(my_app PRIVATE ImGuiNotify::ImGuiNotify)
```

## License

[![MIT license](https://img.shields.io/badge/License-MIT-blue.svg)](https://github.com/TyomaVader/ImGuiNotify/blob/Dev/LICENSE)
