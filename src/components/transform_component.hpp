#ifndef DEBBY_COMPONENTS_TRANSFORM_COMPONENT_HPP_
#define DEBBY_COMPONENTS_TRANSFORM_COMPONENT_HPP_

#include <glm/ext/vector_float2.hpp>

namespace debby {

/* TransformComponent allows an entity a
 * notion of a position in space */
class TransformComponent {
   public:
    glm::vec2 position;
    glm::vec2 scale;
    double rotation;

    explicit TransformComponent(glm::vec2 position = {0, 0},
                                glm::vec2 scale = {1, 1}, double rotation = 0.0)
        : position(position), scale(scale), rotation(rotation) {}

    ~TransformComponent() = default;
};
}  // namespace debby

#endif  // DEBBY_COMPONENTS_TRANSFORM_COMPONENT_HPP_
