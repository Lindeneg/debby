#pragma once

#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>

#include "../components/sprite_component.hpp"
#include "../components/transform_component.hpp"
#include "../ecs/ecs.hpp"
#include "../managers/screen_manager.hpp"

namespace debby {

class RenderSystem : public ecs::System {
   private:
    SDL_Renderer *_renderer = managers::screen::get_renderer();

   public:
    RenderSystem() {
        require_component<TransformComponent>();
        require_component<SpriteComponent>();
    }

    inline void update() {
        for (auto entity : get_entities()) {
            const auto &transform{entity.get_component<TransformComponent>()};
            const auto &sprite{entity.get_component<SpriteComponent>()};

            SDL_Rect rect{static_cast<int>(transform.position.x),
                          static_cast<int>(transform.position.y), sprite.width,
                          sprite.height};

            SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);
            SDL_RenderFillRect(_renderer, &rect);
        }
    }
};
}  // namespace debby

