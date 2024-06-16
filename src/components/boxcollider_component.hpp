#ifndef DEBBY_COMPONENTS_BOXCOLLIDER_COMPONENT_HPP_
#define DEBBY_COMPONENTS_BOXCOLLIDER_COMPONENT_HPP_

#include <glm/ext/vector_float2.hpp>

namespace debby {

/* BoxColliderComponent allows an entity to
 * specify a collidiable rectangle box */
class BoxColliderComponent {
   public:
    int width;
    int height;
    glm::vec2 offset;

    explicit BoxColliderComponent(int width = 0, int height = 0,
                                  glm::vec2 offset = {0, 0})
        : width(width), height(height), offset(offset) {}
};
}  // namespace debby

#endif  // DEBBY_COMPONENTS_BOXCOLLIDER_COMPONENT_HPP_
