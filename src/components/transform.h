#ifndef DEBBY_COMPONENTS_TRANSFORM_H_
#define DEBBY_COMPONENTS_TRANSFORM_H_

#include <glm/ext/vector_float2.hpp>

namespace debby::components {
class Transform {
   public:
    glm::vec2 position;
    glm::vec2 scale;
    double rotation;
};
}  // namespace debby::components

#endif  // DEBBY_COMPONENTS_TRANSFORM_H_
