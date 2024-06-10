#include "ecs.h"

#include <algorithm>

////////////////////////////////////////
//////// ENTITY IMPLEMENTATIONS ////////
////////////////////////////////////////

debby::ecs::Entity::Entity(int id) : _id(id) {}

int debby::ecs::Entity::get_id() const { return _id; }

////////////////////////////////////////
//////// SYSTEM IMPLEMENTATION /////////
////////////////////////////////////////

const debby::ecs::ComponentSignature& debby::ecs::System::get_signature()
    const {
    return _signature;
}

const std::vector<debby::ecs::Entity>& debby::ecs::System::get_entities()
    const {
    return _entities;
}

void debby::ecs::System::add_entity(Entity entity) {
    _entities.push_back(entity);
}

void debby::ecs::System::remove_entity(Entity entity) {
    _entities.erase(
        std::remove_if(_entities.begin(), _entities.end(),
                       [&](Entity other) { return entity == other; }),
        _entities.end());
}

////////////////////////////////////////
/////// REGISTRY IMPLEMENTATIONS ///////
////////////////////////////////////////
