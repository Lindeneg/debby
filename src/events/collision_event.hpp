#ifndef DEBBY_EVENTS_COLLISION_EVENT_HPP_
#define DEBBY_EVENTS_COLLISION_EVENT_HPP_

#include "../ecs/ecs.hpp"
#include "./event.hpp"

namespace debby {
class CollisionEvent : public Event {
   public:
    ecs::Entity a;
    ecs::Entity b;

    CollisionEvent(ecs::Entity a, ecs::Entity b) : a(a), b(b) {}
};
}  // namespace debby

#endif  // DEBBY_EVENTS_COLLISION_EVENT_HPP_
