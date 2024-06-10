#ifndef DEBBY_GLOBALS_H_
#define DEBBY_GLOBALS_H_

#include <SDL2/SDL_pixels.h>

#include <atomic>
#include <glm/vec2.hpp>

namespace debby {

using GameObjectId = std::atomic_uint32_t;
using Color = SDL_Color;

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
constexpr int FPS{60};
constexpr int FRAME_TARGET{1000 / FPS};
constexpr double MAXIMUM_DT{0.05};

constexpr double REL_EPSILON{1e-8};
constexpr double ABS_EPSILON{1e-12};

constexpr std::size_t numOfLayers{7};
}  // namespace debby::constants

namespace debby::color {
constexpr Color white{255, 255, 255, 255};
constexpr Color black{0, 0, 0, 255};
constexpr Color red{255, 0, 0, 255};
constexpr Color green{0, 255, 0, 255};
constexpr Color blue{0, 0, 255, 255};
constexpr Color yellow{255, 255, 0, 255};
constexpr Color cyan{0, 255, 255, 255};
constexpr Color magenta{255, 0, 255, 255};
constexpr Color gray{128, 128, 128, 255};
constexpr Color light_gray{211, 211, 211, 255};
constexpr Color dark_gray{64, 64, 64, 255};
constexpr Color orange{255, 165, 0, 255};
constexpr Color pink{255, 192, 203, 255};
constexpr Color purple{128, 0, 128, 255};
constexpr Color brown{165, 42, 42, 255};
constexpr Color light_blue{173, 216, 230, 255};
constexpr Color light_green{144, 238, 144, 255};
constexpr Color dark_red{139, 0, 0, 255};
constexpr Color dark_green{0, 100, 0, 255};
constexpr Color dark_blue{0, 0, 139, 255};
}  // namespace debby::color

#endif  // DEBBY_GLOBALS_H_
