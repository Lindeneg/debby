#ifndef DEBBY_SYSTEMS_COLLISIONDEBUG_SYSTEM_HPP_
#define DEBBY_SYSTEMS_COLLISIONDEBUG_SYSTEM_HPP_

#include <SDL2/SDL_rect.h>
#include <spdlog/spdlog.h>

#include "../common/globals.hpp"
#include "../components/boxcollider_component.hpp"
#include "../components/transform_component.hpp"
#include "../ecs/ecs.hpp"
#include "../managers/screen_manager.hpp"

namespace debby {

/* CollisionDebugSystem renders Entity box colliders
 *
 * Entities must have the following components:
 * - BoxColliderComponent
 * - TransformComponent */
class CollisionDebugSystem : public ecs::System {
   public:
    CollisionDebugSystem() {
        require_component<BoxColliderComponent>();
        require_component<TransformComponent>();
    }

    inline void update() {
        for (auto entity : get_entities()) {
            auto collider{entity.get_component<BoxColliderComponent>()};
            auto transform{entity.get_component<TransformComponent>()};
            SDL_Rect rect{
                static_cast<int>(transform.position.x),
                static_cast<int>(transform.position.y),
                collider.width * static_cast<int>(transform.scale.x),
                collider.height * static_cast<int>(transform.scale.y)};
            managers::screen::set_draw_color(color::green);
            SDL_RenderDrawRect(managers::screen::get_renderer(), &rect);
        }
    }
};
}  // namespace debby

#endif  // DEBBY_SYSTEMS_COLLISIONDEBUG_SYSTEM_HPP_
