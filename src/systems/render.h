#ifndef DEBBY_SYSTEMS_RENDER_H_
#define DEBBY_SYSTEMS_RENDER_H_

#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>

#include "../common/components.h"
#include "./ecs.h"

namespace debby::systems {

class Render : public ecs::System {
   public:
    Render() {
        require_component<components::Transform>();
        require_component<components::Sprite>();
    }

    inline void update(SDL_Renderer* renderer) {
        for (auto entity : get_entities()) {
            const auto& transform{
                entity.get_component<components::Transform>()};
            const auto& sprite{entity.get_component<components::Sprite>()};

            SDL_Rect rect{static_cast<int>(transform.position.x),
                          static_cast<int>(transform.position.y), sprite.width,
                          sprite.height};

            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderFillRect(renderer, &rect);
        }
    }
};
}  // namespace debby::systems

#endif  // DEBBY_SYSTEMS_RENDER_H_
