#include "ecs.hpp"

#include <spdlog/spdlog.h>

#include <algorithm>

debby::ecs::IdCounter debby::ecs::IComponent::_next_id{};

debby::ecs::Entity::Entity(Id id) : _id(id), registry(nullptr) {}

debby::ecs::Id debby::ecs::Entity::get_id() const { return _id; }

void debby::ecs::Entity::kill() {
    registry->destroy_entity(*this);
    spdlog::debug("destroying entity {0:d}", _id);
}

bool debby::ecs::Entity::operator==(const Entity &other) const {
    return _id == other._id;
}

bool debby::ecs::Entity::operator!=(const Entity &other) const {
    return _id != other._id;
}

bool debby::ecs::Entity::operator<(const Entity &other) const {
    return _id < other._id;
}

bool debby::ecs::Entity::operator>(const Entity &other) const {
    return _id > other._id;
}

const debby::ecs::ComponentSignature &debby::ecs::System::get_signature()
    const {
    return _signature;
}

const std::vector<debby::ecs::Entity> &debby::ecs::System::get_entities()
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
    _entities.erase(iter, _entities.end());
}

debby::ecs::Registry::Registry()
    : _entity_counter({}),
      _component_pools({}),
      _entity_component_signatures({}),
      _systems({}),
      _entities_add_queue({}),
      _entities_remove_queue({}) {}

debby::ecs::Entity debby::ecs::Registry::create_entity() {
    Id entity_id{};
    if (_free_ids.empty()) {
        IdCounter new_id{_entity_counter++};
        entity_id = new_id.load();
        if (entity_id >=
            static_cast<unsigned int>(_entity_component_signatures.size())) {
            // resize by one, since the resizing should be rare
            // so using the default vector resize could be expensive
            _entity_component_signatures.resize(entity_id + 1);
        }
    } else {
        entity_id = _free_ids.front();
        _free_ids.pop_front();
    }
    spdlog::trace("adding entity {0:d} to registry", entity_id);
    Entity entity{entity_id};
    entity.registry = this;
    _entities_add_queue.insert(entity);
    return entity;
}

void debby::ecs::Registry::destroy_entity(Entity entity) {
    _entities_remove_queue.insert(entity);
}

void debby::ecs::Registry::_add_entity_to_systems(Entity entity) {
    const Id entity_id{entity.get_id()};
    const ComponentSignature &entity_component_signature{
        _entity_component_signatures[entity_id]};
    for (auto &system : _systems) {
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

void debby::ecs::Registry::_remove_entity_from_systems(Entity entity) {
    for (auto &system : _systems) {
        // TODO also remove entity components
        spdlog::trace("removing entity {0:d} from {1}", entity.get_id(),
                      system.first.name());
        system.second->remove_entity(entity);
    }
}

void debby::ecs::Registry::update() {
    for (auto entity : _entities_add_queue) {
        _add_entity_to_systems(entity);
    }
    _entities_add_queue.clear();
    for (auto entity : _entities_remove_queue) {
        const Id entity_id{entity.get_id()};
        _remove_entity_from_systems(entity);
        _entity_component_signatures[entity_id].reset();
        _free_ids.push_back(entity_id);
    }
    _entities_remove_queue.clear();
}
