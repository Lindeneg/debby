#pragma once

#include <SDL2/SDL_rect.h>

#include <string>

namespace debby {
class SpriteComponent {
   public:
    std::string asset_id;
    int width;
    int height;
    SDL_Rect src_rect;

    SpriteComponent(std::string asset_id = "", int width = 0, int height = 0,
                    int src_x = 0, int src_y = 0) {
        this->asset_id = asset_id;
        this->width = width;
        this->height = height;
        this->src_rect = {src_x, src_y, width, height};
    }
    ~SpriteComponent() = default;
};

}  // namespace debby
