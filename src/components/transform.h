#ifndef DEBBY_COMPONENTS_TRANSFORM_H_
#define DEBBY_COMPONENTS_TRANSFORM_H_

#include <glm/ext/vector_float2.hpp>

namespace debby::components {

////////////////////////////////////////
//////// TRANSFORM DEFINITION //////////
////////////////////////////////////////

/*
 * Transform */
class Transform {
   public:
    glm::vec2 position;
    glm::vec2 scale;
    double rotation;

    Transform();
    Transform(glm::vec2 position, glm::vec2 scale, double rotation);
    ~Transform() = default;
};
}  // namespace debby::components

#endif  // DEBBY_COMPONENTS_TRANSFORM_H_
