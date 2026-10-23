#pragma once

#include "imgui.h"
#include "../../include/IconsFontAwesome6.h"
#include "../assets/fa-solid-900.h"

inline void LoadNotificationFonts(ImFontAtlas& fonts)
{
    // Explicit reference size keeps the legacy Font Awesome merge valid in ImGui 1.92.
    ImFontConfig defaultConfig;
    defaultConfig.SizePixels = 13.0f;
    fonts.AddFontDefault(&defaultConfig);

    float baseFontSize = 16.0f;
    float iconFontSize = baseFontSize * 2.0f / 3.0f; // FontAwesome fonts need to have their sizes reduced by 2.0f/3.0f in order to align correctly

    static constexpr ImWchar iconsRanges[] = {ICON_MIN_FA, ICON_MAX_16_FA, 0};
    ImFontConfig iconsConfig;
    iconsConfig.MergeMode = true;
    iconsConfig.PixelSnapH = true;
    iconsConfig.GlyphMinAdvanceX = iconFontSize;
    fonts.AddFontFromMemoryCompressedTTF(fa_solid_900_compressed_data, fa_solid_900_compressed_size, iconFontSize, &iconsConfig, iconsRanges);
}
