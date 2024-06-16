#ifndef DEBBY_SYSTEMS_DAMAGE_SYSTEM_HPP_
#define DEBBY_SYSTEMS_DAMAGE_SYSTEM_HPP_

#include <spdlog/spdlog.h>

#include "../components/boxcollider_component.hpp"
#include "../ecs/ecs.hpp"
#include "../events/collision_event.hpp"
#include "../managers/event_manager.hpp"

namespace debby {

class DamageSystem : public ecs::System {
   public:
    DamageSystem() { require_component<BoxColliderComponent>(); }

    inline void subscribe_to_events() {
        EventManager::connect<CollisionEvent>(this,
                                              &DamageSystem::on_collision);
    }

    inline void on_collision(CollisionEvent &event) {
        // TODO remove this test code
        spdlog::debug("DamageSystem: collision {0:d} -> {1:d}",
                      event.a.get_id(), event.b.get_id());
        event.a.kill();
        event.b.kill();
    }

    inline void update() {}
};
}  // namespace debby

#endif  // DEBBY_SYSTEMS_DAMAGE_SYSTEM_HPP_
