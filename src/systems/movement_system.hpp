#pragma once

#include "../components/rigidbody_component.hpp"
#include "../components/transform_component.hpp"
#include "../ecs/ecs.hpp"

namespace debby {

class MovementSystem : public ecs::System {
   public:
    MovementSystem() {
        require_component<TransformComponent>();
        require_component<RigidBodyComponent>();
    }

    inline void update(float dt) {
        for (auto entity : get_entities()) {
            auto& transform{entity.get_component<TransformComponent>()};
            const auto& rigid_body{entity.get_component<RigidBodyComponent>()};

            transform.position += (rigid_body.velocity * dt);
        }
    }
};
}  // namespace debby

