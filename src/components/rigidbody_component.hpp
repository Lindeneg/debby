#ifndef DEBBY_COMPONENTS_RIGIDBODY_COMPONENT_HPP_
#define DEBBY_COMPONENTS_RIGIDBODY_COMPONENT_HPP_

#include <glm/ext/vector_float2.hpp>

namespace debby {

/* RigidBodyComponent allows an
 * entity a notion of movement */
class RigidBodyComponent {
   public:
    glm::vec2 velocity;

    explicit RigidBodyComponent(glm::vec2 velocity = {0, 0})
        : velocity(velocity) {}

    ~RigidBodyComponent() = default;
};

}  // namespace debby

#endif  // DEBBY_COMPONENTS_RIGIDBODY_COMPONENT_HPP_
