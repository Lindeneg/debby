#ifndef DEBBY_SYSTEMS_MOVEMENT_H_
#define DEBBY_SYSTEMS_MOVEMENT_H_

#include "./ecs.h"

namespace debby::systems {
class Movement : public ecs::System {
   public:
    Movement();

    void update();
};
}  // namespace debby::systems

#endif  // DEBBY_SYSTEMS_MOVEMENT_H_
