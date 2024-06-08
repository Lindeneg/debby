#include "utils.h"

debby::Color debby::utils::uint32_to_color(const uint32_t color) noexcept {
    Color c{};
    c.a = static_cast<Uint8>((color >> 24) & 0xFF);
    c.r = static_cast<Uint8>((color >> 16) & 0xFF);
    c.g = static_cast<Uint8>((color >> 8) & 0xFF);
    c.b = static_cast<Uint8>(color & 0xFF);
    return c;
}

uint32_t debby::utils::color_to_uint32(const Color color) noexcept {
    return (uint32_t)((color.a << 24) + (color.r << 16) + (color.g << 8) +
                      (color.b << 0));
}

