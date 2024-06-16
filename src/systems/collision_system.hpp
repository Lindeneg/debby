#ifndef DEBBY_SYSTEMS_COLLISION_SYSTEM_HPP_
#define DEBBY_SYSTEMS_COLLISION_SYSTEM_HPP_

#include <SDL2/SDL_rect.h>
#include <spdlog/spdlog.h>

#include "../components/boxcollider_component.hpp"
#include "../components/transform_component.hpp"
#include "../ecs/ecs.hpp"
#include "../events/collision_event.hpp"
#include "../managers/event_manager.hpp"

namespace debby {

/* CollisionSystem handles Entity collision box detection
 *
 * Entities must have the following components:
 * - BoxColliderComponent
 * - TransformComponent */
class CollisionSystem : public ecs::System {
   public:
    CollisionSystem() {
        require_component<BoxColliderComponent>();
        require_component<TransformComponent>();
    }

    inline void update() {
        auto entities{get_entities()};
        for (auto i = entities.begin(); i != entities.end(); i++) {
            ecs::Entity a{*i};
            auto &a_collider{a.get_component<BoxColliderComponent>()};
            auto a_transform{a.get_component<TransformComponent>()};
            SDL_Rect a_rect{
                static_cast<int>(a_transform.position.x +
                                 a_collider.offset.x * a_transform.scale.x),
                static_cast<int>(a_transform.position.y +
                                 a_collider.offset.y * a_transform.scale.y),
                a_collider.width * static_cast<int>(a_transform.scale.x),
                a_collider.height * static_cast<int>(a_transform.scale.y),
            };
            for (auto j = i; j != entities.end(); j++) {
                ecs::Entity b{*j};
                if (a == b) {
                    continue;
                }
                auto &b_collider{b.get_component<BoxColliderComponent>()};
                auto b_transform{b.get_component<TransformComponent>()};
                SDL_Rect b_rect{
                    static_cast<int>(
                        b_transform.position.x +
                        (b_collider.offset.x * b_transform.scale.x)),
                    static_cast<int>(
                        b_transform.position.y +
                        (b_collider.offset.y * b_transform.scale.y)),
                    b_collider.width * static_cast<int>(b_transform.scale.x),
                    b_collider.height * static_cast<int>(b_transform.scale.y),
                };
                if (SDL_HasIntersection(&a_rect, &b_rect)) {
                    EventManager::emit<CollisionEvent>(a, b);
                }
            }
        }
    }
};
}  // namespace debby

#endif  // DEBBY_SYSTEMS_COLLISION_SYSTEM_HPP_
