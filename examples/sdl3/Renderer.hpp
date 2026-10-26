#pragma once

#include <SDL3/SDL.h>

struct ImDrawData;

namespace Renderer
{
SDL_WindowFlags WindowFlags();
bool Init(SDL_Window* window);
void NewFrame();
bool Render(ImDrawData* data);
void Present();
// Also releases partial initialization and waits for submitted GPU work.
void Shutdown();
}
