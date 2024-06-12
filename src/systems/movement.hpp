#ifndef DEBBY_SYSTEMS_MOVEMENT_H_
#define DEBBY_SYSTEMS_MOVEMENT_H_

#include "../common/components.hpp"
#include "../ecs/ecs.hpp"

namespace debby::systems {

class Movement : public ecs::System {
   public:
    Movement() {
        require_component<components::Transform>();
        require_component<components::RigidBody>();
    }

    inline void update(float dt) {
        for (auto entity : get_entities()) {
            auto& transform{entity.get_component<components::Transform>()};
            const auto& rigid_body{
                entity.get_component<components::RigidBody>()};

            transform.position += (rigid_body.velocity * dt);
        }
    }
};
}  // namespace debby::systems

#endif  // DEBBY_SYSTEMS_MOVEMENT_H_
