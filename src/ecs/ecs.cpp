#include "ecs.hpp"

#include <spdlog/spdlog.h>

#include <algorithm>

////////////////////////////////////////
/////// COMPONENT IMPLEMENTATION ///////
////////////////////////////////////////

debby::ecs::IdCounter debby::ecs::IComponent::_next_id{};

////////////////////////////////////////
//////// ENTITY IMPLEMENTATION /////////
////////////////////////////////////////

debby::ecs::Entity::Entity(Id id) : _id(id) {}

debby::ecs::Id debby::ecs::Entity::get_id() const { return _id; }

bool debby::ecs::Entity::operator==(const Entity& other) const {
    return _id == other._id;
}

bool debby::ecs::Entity::operator!=(const Entity& other) const {
    return _id != other._id;
}

bool debby::ecs::Entity::operator<(const Entity& other) const {
    return _id < other._id;
}

bool debby::ecs::Entity::operator>(const Entity& other) const {
    return _id > other._id;
}

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
    auto iter{std::remove_if(_entities.begin(), _entities.end(),
                             [&](Entity other) { return entity == other; })};
    if (iter == _entities.end()) {
        spdlog::warn("tried to remove non-existent entity {0:d} from registry",
                     entity.get_id());
        return;
    }
    spdlog::trace("removing entity {0:d} from registry", entity.get_id());
    _entities.erase(iter, _entities.end());
}

////////////////////////////////////////
/////// REGISTRY IMPLEMENTATION ////////
////////////////////////////////////////

debby::ecs::Registry::Registry()
    : _entity_counter({}),
      _component_pools({}),
      _entity_component_signatures({}),
      _systems({}),
      _entities_add_queue({}),
      _entities_remove_queue({}) {}

debby::ecs::Entity debby::ecs::Registry::create_entity() {
    IdCounter new_id{_entity_counter++};
    Id entity_id{new_id.load()};
    spdlog::trace("adding entity {0:d} to registry", entity_id);
    if (entity_id >=
        static_cast<unsigned int>(_entity_component_signatures.size())) {
        // resize by one, since the resizing should be rare
        // so using the default vector resize could be expensive
        _entity_component_signatures.resize(entity_id + 1);
    }
    Entity entity{entity_id};
    entity.registry = this;
    _entities_add_queue.insert(entity);
    return entity;
}

void debby::ecs::Registry::add_entity_to_systems(Entity entity) {
    const Id entity_id{entity.get_id()};
    const ComponentSignature& entity_component_signature{
        _entity_component_signatures[entity_id]};
    for (auto& system : _systems) {
        const auto system_component_signature{system.second->get_signature()};
        const bool is_interested{
            (entity_component_signature & system_component_signature) ==
            system_component_signature};
        if (is_interested) {
            spdlog::trace("adding entity {0:d} to {1}", entity_id,
                          system.first.name());
            system.second->add_entity(entity);
        }
    }
}

void debby::ecs::Registry::update() {
    for (auto entity : _entities_add_queue) {
        add_entity_to_systems(entity);
    }
    _entities_add_queue.clear();
}
