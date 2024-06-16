#ifndef DEBBY_SYSTEMS_RENDER_SYSTEM_HPP_
#define DEBBY_SYSTEMS_RENDER_SYSTEM_HPP_

#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <spdlog/spdlog.h>

#include <algorithm>

#include "../components/sprite_component.hpp"
#include "../components/transform_component.hpp"
#include "../ecs/ecs.hpp"
#include "../managers/asset_manager.hpp"
#include "../managers/screen_manager.hpp"

namespace debby {

/* RenderSystem handles Entity rendering
 *
 * Entities must have the following components:
 * - TransformComponent
 * - SpriteComponent */
class RenderSystem : public ecs::System {
   private:
    SDL_Renderer *_renderer = managers::screen::get_renderer();

   public:
    RenderSystem() {
        require_component<TransformComponent>();
        require_component<SpriteComponent>();
    }

    inline void update() {
        // TODO should probably not sort on each frame
        auto entities{get_entities()};
        std::sort(entities.begin(), entities.end(),
                  [&](const ecs::Entity a, const ecs::Entity b) {
                      return a.get_component<SpriteComponent>().z_index <
                             b.get_component<SpriteComponent>().z_index;
                  });
        for (auto entity : entities) {
            const auto &sprite{entity.get_component<SpriteComponent>()};
            const auto &texture{managers::asset::get_texture(sprite.asset_id)};
            if (!texture) {
                // TODO can probably check if a texture exists earlier on
                spdlog::warn("texture {0} not found on entity {1:d}",
                             sprite.asset_id, entity.get_id());
                continue;
            }
            const auto &transform{entity.get_component<TransformComponent>()};

            SDL_Rect src_rect{sprite.get_rect()};
            SDL_Rect dst_rect{
                static_cast<int>(transform.position.x),
                static_cast<int>(transform.position.y),
                sprite.width * static_cast<int>(transform.scale.x),
                sprite.height * static_cast<int>(transform.scale.y)};

            SDL_RenderCopyEx(_renderer, texture, &src_rect, &dst_rect,
                             transform.rotation, nullptr, SDL_FLIP_NONE);
        }
    }
};
}  // namespace debby

#endif  // DEBBY_SYSTEMS_RENDER_SYSTEM_HPP_
