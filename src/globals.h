#ifndef DEBBY_GLOBALS_H_
#define DEBBY_GLOBALS_H_

#include <SDL2/SDL_pixels.h>

#include <atomic>
#include <string_view>

namespace debby {
using GameObjectName = std::string;
using GameObjectTag = std::string_view;
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
constexpr Color White{255, 255, 255, 255};
constexpr Color Black{0, 0, 0, 255};
constexpr Color Red{255, 0, 0, 255};
constexpr Color Green{0, 255, 0, 255};
constexpr Color Blue{0, 0, 255, 255};
constexpr Color Yellow{255, 255, 0, 255};
constexpr Color Cyan{0, 255, 255, 255};
constexpr Color Magenta{255, 0, 255, 255};
constexpr Color Gray{128, 128, 128, 255};
constexpr Color LightGray{211, 211, 211, 255};
constexpr Color DarkGray{64, 64, 64, 255};
constexpr Color Orange{255, 165, 0, 255};
constexpr Color Pink{255, 192, 203, 255};
constexpr Color Purple{128, 0, 128, 255};
constexpr Color Brown{165, 42, 42, 255};
constexpr Color LightBlue{173, 216, 230, 255};
constexpr Color LightGreen{144, 238, 144, 255};
constexpr Color DarkRed{139, 0, 0, 255};
constexpr Color DarkGreen{0, 100, 0, 255};
constexpr Color DarkBlue{0, 0, 139, 255};
}  // namespace debby::color

#endif  // DEBBY_GLOBALS_H_
