#include "Demo.hpp"
#include "Fonts.hpp"
#include "imgui.h"
#include "Renderer.hpp"
#include "imgui_impl_sdl3.h"
#include <SDL3/SDL_main.h>

#include <cstdio>
#include <stdexcept>

int main(int, char**)
{
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
    ImGuiNotifyDemo::ApplyTheme();
    ImGui::GetStyle().ScaleAllSizes(scale);
    ImGui::GetStyle().FontScaleDpi = scale;

    int result = 0;
    try
    {
        if (!Renderer::Init(window))
            throw std::runtime_error(SDL_GetError());
        LoadNotificationFonts(*io.Fonts);

        bool done = false;
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
                ImGui::Render();
                Renderer::Render(ImGui::GetDrawData());
                ImGui::UpdatePlatformWindows();
                ImGui::RenderPlatformWindowsDefault();
                Renderer::Present();
            }
        }
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
