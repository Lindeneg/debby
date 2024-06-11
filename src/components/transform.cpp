#include "transform.h"

////////////////////////////////////////
////// TRANSFORM IMPLEMENTATION ////////
////////////////////////////////////////

debby::components::Transform::Transform()
    : Transform(glm::vec2(0, 0), glm::vec2(1, 1), 0.0) {}

debby::components::Transform::Transform(glm::vec2 position, glm::vec2 scale,
                                        double rotation) {
    this->position = position;
    this->scale = scale;
    this->rotation = rotation;
}

debby::components::Transform::~Transform() {}
