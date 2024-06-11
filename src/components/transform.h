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

    Transform(glm::vec2 position = glm::vec2(0, 0),
              glm::vec2 scale = glm::vec2(1, 1), double rotation = 0.0) {
        this->position = position;
        this->scale = scale;
        this->rotation = rotation;
    }
    ~Transform() = default;
};
}  // namespace debby::components

#endif  // DEBBY_COMPONENTS_TRANSFORM_H_
