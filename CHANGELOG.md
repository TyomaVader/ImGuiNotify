# Changelog

## [0.0.3] - 07.07.2024

### Added
- Optional notifications rendering relative to the monitor size (See ```NOTIFY_RENDER_OUTSIDE_MAIN_WINDOW```) ([#7](https://github.com/TyomaVader/ImGuiNotify/issues/7))
- Simultanious notification rendering limit (See ```NOTIFY_RENDER_LIMIT```)
- MacOS support ([2199194](https://github.com/TyomaVader/ImGuiNotify/commit/2199194b03fe82892e642ff9920ae41ac542c2d5) & [a3eb32b](https://github.com/TyomaVader/ImGuiNotify/commit/a3eb32bc63e0433d16c0bf2e2e080303889c5737)) \
Thanks to [@sjy0727](https://github.com/sjy0727) and [@starlight-traveler](https://github.com/starlight-traveler)

### Changed
- README clarifications. ([#5](https://github.com/TyomaVader/ImGuiNotify/issues/5))
- Upgraded Dear ImGui version used in example to [v1.90.9](https://github.com/ocornut/imgui/releases/tag/v1.90.9)
- Upgraded Font Awesome version

### Fixed
- Removed unnecessary files. ([#5](https://github.com/TyomaVader/ImGuiNotify/issues/5))
- Warnings fixed in Windows example.

## [0.0.2] - 30.08.2023

### Added
- CMake support, see CMakeLists.txt for details
- Dismiss button for notifications (optional)
- Optional button in the notification that executes a user-defined function
- GitHub Actions for Linux and Windows builds
- Documentation examples

### Fixed
- Documentation fixes

### Changed
- Switched from classic enums to scoped enums
- Upgraded Dear ImGui version used in example to [v1.89.9 WIP](https://github.com/ocornut/imgui/commit/11613013860d149667302a258041dcd832069f36)

## [0.0.1] - 28.07.2023

### Added
- Linux support
- Documentation

### Fixed
- Notifications now render above all other windows
- Notifications now render in the correct position when the main window is moved
- Compilation warnings about incorrect usage of ```ImGui::Text()```

### Changed
- Code readability improved
- Switched to Font Awesome 6 icons
- Visual changes to the notifications (can be customized in the ```main.cpp``` file)
- Default ImGui theme changed to *Embrace The Darkness* by [@janekb04](https://github.com/janekb04)


[0.0.3]: https://github.com/TyomaVader/ImGuiNotify/releases/tag/v0.0.3
[0.0.2]: https://github.com/TyomaVader/ImGuiNotify/releases/tag/v0.0.2
[0.0.1]: https://github.com/TyomaVader/ImGuiNotify/releases/tag/v0.0.1