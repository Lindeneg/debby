#ifndef DEBBY_GLOBALS_H_
#define DEBBY_GLOBALS_H_

#include <SDL2/SDL_pixels.h>

#include <atomic>
#include <glm/vec2.hpp>
#include <string_view>

namespace debby {

struct Context {
    float delta_time;
    glm::ivec2 display_size;
    glm::ivec2 resolution;
};

using GameObjectName = std::string;
using GameObjectTag = std::string_view;
using GameObjectId = std::atomic_uint32_t;
using Color = uint32_t;
using SColor = SDL_Color;

enum LayerType {
    TILEMAP_LAYER,
    TERRAIN_LAYER,
    ENEMY_LAYER,
    COLLIDABLE_LAYER,
    PLAYER_LAYER,
    PROJECTILE_LAYER,
    UI_LAYER
};
}  // namespace debby

namespace debby::constants {
constexpr int PIXEL_FORMAT{SDL_PIXELFORMAT_ARGB8888};

constexpr int FPS{60};

constexpr int FRAME_TARGET{1000 / FPS};

constexpr double PI{3.14159265359};

constexpr double TWO_PI{PI * 2.0f};

constexpr double REL_EPSILION{1e-8};

constexpr double ABS_EPSILION{1e-12};

constexpr GameObjectTag DEFAULT_TAG{"Default"};
constexpr GameObjectTag PLAYER_TAG{"Player"};
constexpr GameObjectTag ENEMY_TAG{"Enemey"};

constexpr std::size_t numOfLayers{7};
}  // namespace debby::constants

namespace debby::color {
constexpr SColor s_white{255, 255, 255, 255};
constexpr SColor s_black{0, 0, 0, 255};
constexpr SColor s_red{255, 0, 0, 255};
constexpr SColor s_green{0, 255, 0, 255};
constexpr SColor s_blue{0, 0, 255, 255};
constexpr SColor s_yellow{255, 255, 0, 255};
constexpr SColor s_cyan{0, 255, 255, 255};
constexpr SColor s_magenta{255, 0, 255, 255};
constexpr SColor s_gray{128, 128, 128, 255};
constexpr SColor s_light_gray{211, 211, 211, 255};
constexpr SColor s_dark_gray{64, 64, 64, 255};
constexpr SColor s_orange{255, 165, 0, 255};
constexpr SColor s_pink{255, 192, 203, 255};
constexpr SColor s_purple{128, 0, 128, 255};
constexpr SColor s_brown{165, 42, 42, 255};
constexpr SColor s_light_blue{173, 216, 230, 255};
constexpr SColor s_light_green{144, 238, 144, 255};
constexpr SColor s_dark_red{139, 0, 0, 255};
constexpr SColor s_dark_green{0, 100, 0, 255};
constexpr SColor s_dark_blue{0, 0, 139, 255};

constexpr Color white{0xFFFFFFFF};
constexpr Color black{0xFF000000};
constexpr Color red{0xFFFF0000};
constexpr Color green{0xFF00FF00};
constexpr Color blue{0xFF0000FF};
constexpr Color yellow{0xFFFFFF00};
constexpr Color cyan{0xFF00FFFF};
constexpr Color magenta{0xFFFF00FF};
constexpr Color gray{0xFF808080};
constexpr Color light_gray{0xFFD3D3D3};
constexpr Color dark_gray{0xFF404040};
constexpr Color orange{0xFFFFA500};
constexpr Color pink{0xFFFFC0CB};
constexpr Color purple{0xFF800080};
constexpr Color brown{0xFFA52A2A};
constexpr Color light_blue{0xFFADD8E6};
constexpr Color light_green{0xFF90EE90};
constexpr Color dark_red{0xFF8B0000};
constexpr Color dark_green{0xFF006400};
constexpr Color dark_blue{0xFF00008B};
}  // namespace debby::color

#endif  // DEBBY_GLOBALS_H_
