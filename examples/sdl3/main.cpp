#include "Demo.hpp"
#include "Fonts.hpp"
#include "ImGuiNotify.hpp"
#include "Renderer.hpp"
#include "imgui_impl_sdl3.h"
#include <SDL3/SDL_main.h>

#include <cstdio>
#include <cstring>
#include <stdexcept>

int main(int argc, char** argv)
{
    const bool smokeTest = argc == 2 && std::strcmp(argv[1], "--smoke-test") == 0;
    if (argc > 1 && !smokeTest)
    {
        std::fprintf(stderr, "Usage: %s [--smoke-test]\n", argv[0]);
        return 1;
    }
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD))
    {
        std::fprintf(stderr, "SDL_Init: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    const float scale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
    SDL_Window* window = SDL_CreateWindow("ImGuiNotify", int(1000 * scale), int(700 * scale),
        SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY | Renderer::WindowFlags());
    if (!window)
    {
        std::fprintf(stderr, "SDL_CreateWindow: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard | ImGuiConfigFlags_NavEnableGamepad |
        ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_ViewportsEnable;
    io.ConfigDpiScaleFonts = true;
    io.ConfigDpiScaleViewports = true;
    if (smokeTest)
        io.IniFilename = nullptr;
    ImGuiNotifyDemo::ApplyTheme();
    ImGui::GetStyle().ScaleAllSizes(scale);
    ImGui::GetStyle().FontScaleDpi = scale;

    int result = 0;
    try
    {
        if (!Renderer::Init(window))
            throw std::runtime_error(SDL_GetError());
        LoadNotificationFonts(*io.Fonts);
        if (smokeTest)
            ImGui::InsertNotification({ImGuiToastType::Success, 60000, "SDL3 native renderer smoke test"});

        bool done = false;
        int frames = 0;
        int renderedFrames = 0;
        const int resizedWidth = int(900 * scale);
        const int resizedHeight = int(650 * scale);
        while (!done)
        {
#ifdef __APPLE__
            @autoreleasepool
#endif
            {
                SDL_Event event;
                while (SDL_PollEvent(&event))
                {
                    ImGui_ImplSDL3_ProcessEvent(&event);
                    if (event.type == SDL_EVENT_QUIT ||
                        (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED &&
                         event.window.windowID == SDL_GetWindowID(window)))
                        done = true;
                }
                if (done)
                    break;
                if (SDL_GetWindowFlags(window) & SDL_WINDOW_MINIMIZED)
                {
                    SDL_Delay(10);
                    continue;
                }

                Renderer::NewFrame();
                ImGui_ImplSDL3_NewFrame();
                ImGui::NewFrame();
                ImGuiNotifyDemo::DrawControls();
                ImGuiNotifyDemo::DrawNotifications();
                if (smokeTest)
                {
                    const auto* viewport = ImGui::GetMainViewport();
                    ImGuiWindowClass windowClass;
                    windowClass.ViewportFlagsOverrideSet = ImGuiViewportFlags_NoAutoMerge;
                    ImGui::SetNextWindowClass(&windowClass);
                    ImGui::SetNextWindowPos(ImVec2(viewport->Pos.x - 40, viewport->Pos.y - 40));
                    ImGui::SetNextWindowSize(ImVec2(240, 100));
                    ImGui::Begin("Secondary viewport smoke test");
                    ImGui::TextUnformatted("Native window and renderer");
                    ImGui::End();
                }
                ImGui::Render();
                if (Renderer::Render(ImGui::GetDrawData()))
                    ++renderedFrames;
                ImGui::UpdatePlatformWindows();
                ImGui::RenderPlatformWindowsDefault();
                Renderer::Present();

                if (smokeTest && ++frames == 4)
                {
                    if (!SDL_SetWindowSize(window, resizedWidth, resizedHeight))
                        throw std::runtime_error(SDL_GetError());
                }
                if (smokeTest && frames == 12)
                {
                    auto* font = ImGui::GetFont();
                    const ImWchar icons[] = {0xf058, 0xf071, 0xf06a, 0xf05a, 0xf00d};
                    for (const auto icon : icons)
                        if (!font->IsGlyphInFont(icon))
                            throw std::runtime_error("Smoke test is missing a notification or dismissal icon glyph");

                    int width = 0, height = 0;
                    if (!SDL_GetWindowSize(window, &width, &height))
                        throw std::runtime_error(SDL_GetError());
                    const ImVec2 drawSize = ImGui::GetDrawData()->DisplaySize;
                    if (width != resizedWidth || height != resizedHeight ||
                        drawSize.x != resizedWidth || drawSize.y != resizedHeight)
                        throw std::runtime_error("Smoke test did not render at the resized window dimensions");

                    const auto* toast = ImGui::FindWindowByName("##TOAST0");
                    if (!toast || !toast->Active || toast->Hidden || toast->DrawList->VtxBuffer.empty() ||
                        !toast->Viewport->DrawData || !toast->Viewport->DrawData->CmdLists.contains(toast->DrawList))
                        throw std::runtime_error("Smoke test did not render notification geometry");

                    int secondaryVertices = 0;
                    for (const auto* viewport : ImGui::GetPlatformIO().Viewports)
                        if (viewport != ImGui::GetMainViewport() && viewport->DrawData)
                            secondaryVertices += viewport->DrawData->TotalVtxCount;
                    const int mainVertices = ImGui::GetDrawData()->TotalVtxCount;
                    if (renderedFrames < 8 || ImGui::GetPlatformIO().Viewports.Size < 2 ||
                        mainVertices == 0 || secondaryVertices == 0)
                    {
                        std::fprintf(stderr, "Rendered frames: %d, viewports: %d, main/secondary vertices: %d/%d\n",
                            renderedFrames, ImGui::GetPlatformIO().Viewports.Size,
                            mainVertices, secondaryVertices);
                        throw std::runtime_error("Smoke test did not render frames and a secondary viewport");
                    }
                    std::printf("PASS: native frames, resize, fonts, notifications and secondary viewport\n");
                    done = true;
                }
            }
        }
        if (smokeTest && frames != 12)
            throw std::runtime_error("Smoke test interrupted before all frames rendered");
    }
    catch (const std::exception& error)
    {
        std::fprintf(stderr, "ImGuiNotify: %s\n", error.what());
        result = 1;
    }

    Renderer::Shutdown();
    if (io.BackendPlatformUserData)
        ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
    SDL_DestroyWindow(window);
    SDL_Quit();
    return result;
}
