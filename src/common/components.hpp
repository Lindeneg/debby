#ifndef DEBBY_COMMON_COMPONENTS_H_
#define DEBBY_COMMON_COMPONENTS_H_

#include <glm/ext/vector_float2.hpp>
#include <string>

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

////////////////////////////////////////
//////// RIGIDBODY DEFINITION //////////
////////////////////////////////////////

/*
 * RigidBody */
class RigidBody {
   public:
    glm::vec2 velocity;

    RigidBody(glm::vec2 velocity = glm::vec2(0, 0)) {
        this->velocity = velocity;
    }
    ~RigidBody() = default;
};

////////////////////////////////////////
///////// SPRITE DEFINITION ////////////
////////////////////////////////////////

/*
 * Sprite */
class Sprite {
   public:
    std::string asset_id;
    int width;
    int height;

    Sprite(std::string asset_id = "", int width = 0, int height = 0) {
        this->asset_id = asset_id;
        this->width = width;
        this->height = height;
    }
    ~Sprite() = default;
};
}  // namespace debby::components

#endif  // DEBBY_COMMON_COMPONENTS_H_
