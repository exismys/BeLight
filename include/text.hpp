#pragma once

#include <filesystem>
#include <string_view>
#include <vector>

#include <stb_truetype.h>

#include "renderer.hpp"
#include "types.hpp"

class Text {
public:
    explicit Text(const std::filesystem::path& path);

    void draw_text(Renderer& renderer, std::string_view text, IVec2 point, float size, Color color);

private:
    stbtt_fontinfo font_;
    std::vector<unsigned char> font_buffer_;
};