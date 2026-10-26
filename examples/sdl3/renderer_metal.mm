// Based on Dear ImGui's SDL3/Metal example. See third_party/licenses/DearImGui.txt.
#include "Renderer.hpp"
#include "imgui.h"
#include "imgui_impl_metal.h"
#include "imgui_impl_sdl3.h"
#include <stdexcept>

#import <Metal/Metal.h>
#import <QuartzCore/CAMetalLayer.h>

namespace
{
SDL_Window* window = nullptr;
SDL_MetalView view = nullptr;
CAMetalLayer* layer = nil;
id<MTLCommandQueue> queue = nil;
id<MTLCommandBuffer> commandBuffer = nil;
id<CAMetalDrawable> drawable = nil;
MTLRenderPassDescriptor* pass = nil;
}

SDL_WindowFlags Renderer::WindowFlags()
{
    return SDL_WINDOW_METAL;
}

bool Renderer::Init(SDL_Window* target)
{
    window = target;
    id<MTLDevice> device = MTLCreateSystemDefaultDevice();
    if (!device)
        return SDL_SetError("No Metal device available");
    view = SDL_Metal_CreateView(window);
    if (!view)
        return false;
    layer = (__bridge CAMetalLayer*)SDL_Metal_GetLayer(view);
    if (!layer)
        return SDL_SetError("SDL did not create a Metal layer");
    layer.device = device;
    layer.pixelFormat = MTLPixelFormatBGRA8Unorm;
    queue = [device newCommandQueue];
    if (!queue)
        return SDL_SetError("Metal command queue creation failed");
    pass = [MTLRenderPassDescriptor new];
    pass.colorAttachments[0].loadAction = MTLLoadActionClear;
    pass.colorAttachments[0].storeAction = MTLStoreActionStore;
    pass.colorAttachments[0].clearColor = MTLClearColorMake(0.025, 0.025, 0.025, 1.0);
    // NewFrame needs a texture format even before the first drawable is acquired.
    MTLTextureDescriptor* descriptor = [MTLTextureDescriptor texture2DDescriptorWithPixelFormat:
        layer.pixelFormat width:1 height:1 mipmapped:NO];
    descriptor.usage = MTLTextureUsageRenderTarget;
    pass.colorAttachments[0].texture = [device newTextureWithDescriptor:descriptor];
    if (!pass.colorAttachments[0].texture)
        return SDL_SetError("Metal render target creation failed");
    return ImGui_ImplMetal_Init(device) && ImGui_ImplSDL3_InitForMetal(window);
}

void Renderer::NewFrame()
{
    ImGui_ImplMetal_NewFrame(pass);
}

bool Renderer::Render(ImDrawData* data)
{
    int width = 0, height = 0;
    SDL_GetWindowSizeInPixels(window, &width, &height);
    if (width <= 0 || height <= 0)
        return false;
    layer.drawableSize = CGSizeMake(width, height);
    drawable = [layer nextDrawable];
    if (!drawable)
        return false;
    commandBuffer = [queue commandBuffer];
    if (!commandBuffer)
        throw std::runtime_error("Metal command buffer creation failed");
    pass.colorAttachments[0].texture = drawable.texture;
    id<MTLRenderCommandEncoder> encoder = [commandBuffer renderCommandEncoderWithDescriptor:pass];
    if (!encoder)
        throw std::runtime_error("Metal render encoder creation failed");
    ImGui_ImplMetal_RenderDrawData(data, commandBuffer, encoder);
    [encoder endEncoding];
    return true;
}

void Renderer::Present()
{
    if (!drawable)
        return;
    [commandBuffer presentDrawable:drawable];
    [commandBuffer commit];
    drawable = nil;
    commandBuffer = nil;
}

void Renderer::Shutdown()
{
    if (queue)
    {
        id<MTLCommandBuffer> completion = [queue commandBuffer];
        [completion commit];
        [completion waitUntilCompleted];
    }
    if (ImGui::GetIO().BackendRendererUserData)
        ImGui_ImplMetal_Shutdown();
    pass = nil;
    commandBuffer = nil;
    drawable = nil;
    queue = nil;
    layer = nil;
    if (view)
        SDL_Metal_DestroyView(view);
    view = nullptr;
    window = nullptr;
}
