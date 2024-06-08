#include "utils.h"

debby::SColor debby::utils::color_to_s_color(const Color color) noexcept {
    SColor c{};
    c.a = static_cast<Uint8>((color >> 24) & 0xFF);
    c.r = static_cast<Uint8>((color >> 16) & 0xFF);
    c.g = static_cast<Uint8>((color >> 8) & 0xFF);
    c.b = static_cast<Uint8>(color & 0xFF);
    return c;
}

debby::Color debby::utils::s_color_to_color(const SColor color) noexcept {
    return static_cast<Color>((color.a << 24) + (color.r << 16) +
                              (color.g << 8) + (color.b << 0));
}

