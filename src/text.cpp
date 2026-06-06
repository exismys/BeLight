#include <fstream>
#include <stdexcept>
#include <vector>

#include <stb_truetype.h>

#include "renderer.hpp"
#include "types.hpp"
#include "text.hpp"

Text::Text(const std::filesystem::path& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Failed to open font");
    }

    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    file.seekg(0);

    font_buffer_.resize(size);

    file.read(reinterpret_cast<char*>(font_buffer_.data()), size);

    if (!stbtt_InitFont(&font_, font_buffer_.data(), stbtt_GetFontOffsetForIndex(font_buffer_.data(), 0))) {
        throw std::runtime_error("Failed to initialize font");
    }
}

void Text::draw_text(Renderer& renderer, std::string_view text, IVec2 point, float size, Color color) {
    float scale = stbtt_ScaleForPixelHeight(&font_, size);

    int cursor_x = point.x;

    for (char c: text) {
        int width;
        int height;

        int xoff;
        int yoff;

        // Todo: Cache this bitmap character-wise
        unsigned char* bitmap  = stbtt_GetCodepointBitmap(&font_, 0, scale, c, &width, &height, &xoff, &yoff);

        for (int gy = 0; gy < height; gy++) {
            for (int gx = 0; gx < width; gx++) {
                unsigned char alpha = bitmap[gy * width + gx];

                if (alpha == 0) continue;
                
                int px = cursor_x + xoff + gx;
                int py = point.y + yoff + gy;

                if (px < 0 || py < 0 || px >= renderer.width || py >= renderer.height) {
                    continue;
                }

                put_pixel(renderer, IVec2{px, py}, color);
            }
        }

        int advance_width;
        int lsb;

        stbtt_GetCodepointHMetrics(&font_, c, &advance_width, &lsb);

        cursor_x += static_cast<int>(advance_width * scale);

        stbtt_FreeBitmap(bitmap, nullptr);
    }
}
