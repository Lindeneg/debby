#ifndef DEBBY_SYSTEMS_ECS_H_
#define DEBBY_SYSTEMS_ECS_H_

#include <spdlog/spdlog.h>

#include <bitset>
#include <memory>
#include <set>
#include <typeindex>
#include <unordered_map>
#include <utility>
#include <vector>

namespace debby::ecs {
// max number of components an entity can have
constexpr unsigned int MAX_COMPONENTS{32};

// describes which component(s) are enabled on an entity
typedef std::bitset<MAX_COMPONENTS> ComponentSignature;

////////////////////////////////////////
//////// COMPONENT DEFINITION //////////
////////////////////////////////////////

class IComponent {
   protected:
    static int _next_id;
};

/*
 * Component */
template <typename T>
class Component : public IComponent {
    friend class System;
    friend class Registry;

   private:
    inline static int _get_id() {
        static int id{_next_id++};
        return id;
    }
};

////////////////////////////////////////
///////// ENTITY DEFINITION ////////////
////////////////////////////////////////

/*
 * Entity is effectively just an identifier used by a system
 * and an registry to register components and process behavior */
class Entity {
   private:
    int _id;

   public:
    class Registry *registry;

    Entity(int id);
    ~Entity() = default;

    int get_id() const;

    bool operator==(const Entity &other) const;
    bool operator!=(const Entity &other) const;
    bool operator<(const Entity &other) const;
    bool operator>(const Entity &other) const;

    template <typename TComponent, typename... TComponentArgs>
    void add_component(TComponentArgs &&...args);

    template <typename TComponent>
    void remove_component();

    template <typename TComponent>
    bool has_component() const;

    template <typename TComponent>
    TComponent &get_component() const;
};

////////////////////////////////////////
////////// SYSTEM DEFINITION ///////////
////////////////////////////////////////

/*
 * System processes entities that
 * contain a specific signature */
class System {
   private:
    ComponentSignature _signature;
    std::vector<Entity> _entities;

   public:
    System() = default;
    ~System() = default;

    const ComponentSignature &get_signature() const;

    const std::vector<Entity> &get_entities() const;

    void add_entity(Entity entity);

    void remove_entity(Entity entity);

    // defines the component that an entity must have
    // in order to be considered by the system
    template <typename TComponent>
    inline void require_component() {
        const auto component_id{Component<TComponent>::_get_id()};
        _signature.set(component_id);
    }
};

////////////////////////////////////////
/////////// POOL DEFINITION ////////////
////////////////////////////////////////

class IPool {
   public:
    virtual ~IPool() {}
};

/*
 * Pool is a vector wrapper of type T objects */
template <typename T>
class Pool : public IPool {
   private:
    std::vector<T> _data;

   public:
    Pool(unsigned int size = 100) { _data.resize(size); }

    virtual ~Pool() = default;

    inline bool is_empty() const { return _data.empty(); }

    inline int get_size() const { return static_cast<int>(_data.size()); }

    inline void resize(int new_size) { _data.resize(new_size); }

    inline void clear() { _data.clear(); }

    inline void add(T obj) { _data.push_back(obj); }

    inline void set(unsigned int index, T object) { _data[index] = object; }

    inline T &get(unsigned int index) { return _data[index]; }

    inline T &operator[](unsigned int index) { return _data[index]; }
};

////////////////////////////////////////
///////// REGISTRY DEFINITION //////////
////////////////////////////////////////

/*
 * Registry manages creation and destruction of entities,
 * adding systems and adding components to entities
 * */
class Registry {
   private:
    int _num_entities;

    // each pool contains all data for
    // a certain component type.
    // Vector index is component id and
    // pool index is entity id
    std::vector<std::shared_ptr<IPool>> _component_pools;

    // vector of component signatures per entity
    // vector index is equal to entity id
    std::vector<ComponentSignature> _entity_component_signatures;

    std::unordered_map<std::type_index, std::shared_ptr<System>> _systems;

    // save entities to add/remove, such that they can
    // be processed in bulk at the end of each frame
    std::set<Entity> _entities_add_queue;
    std::set<Entity> _entities_remove_queue;

   public:
    Registry();
    ~Registry() = default;

    void update();

    ////////////////////////////////////////
    ////////////// ENTITIES ////////////////
    ////////////////////////////////////////

    Entity create_entity();

    ////////////////////////////////////////
    ///////////// COMPONENTS ///////////////
    ////////////////////////////////////////

    template <typename TComponent, typename... TComponentArgs>
    inline void add_component(Entity entity, TComponentArgs &&...args) {
        const int component_id{Component<TComponent>::_get_id()};
        if (component_id >= static_cast<int>(_component_pools.size())) {
            // resize by one, since the resizing should be rare
            // so using the default vector resize could be expensive
            _component_pools.resize(component_id + 1, nullptr);
        }
        if (!_component_pools[component_id]) {
            _component_pools[component_id] =
                std::make_shared<Pool<TComponent>>();
        }
        std::shared_ptr<Pool<TComponent>> component_pool{
            std::static_pointer_cast<Pool<TComponent>>(
                _component_pools[component_id])};
        const int entity_id{entity.get_id()};
        if (entity_id >= component_pool->get_size()) {
            component_pool->resize(_num_entities);
        }
        spdlog::debug("adding component {0:d} to entity {1:d}", component_id,
                      entity_id);
        TComponent new_component(std::forward<TComponentArgs>(args)...);
        component_pool->set(entity_id, new_component);
        _entity_component_signatures[entity_id].set(component_id);
    }

    template <typename TComponent>
    inline void remove_component(Entity entity) {
        const int component_id{Component<TComponent>::_get_id()};
        const int entity_id{entity.get_id()};
        spdlog::debug("removing component {0:d} from entity {1:d}",
                      component_id, entity_id);
        _entity_component_signatures[entity_id].set(component_id, false);
    }

    template <typename TComponent>
    inline bool has_component(Entity entity) const {
        const int component_id{Component<TComponent>::_get_id()};
        const int entity_id{entity.get_id()};
        return _entity_component_signatures[entity_id].test(component_id);
    }

    template <typename TComponent>
    inline TComponent &get_component(Entity entity) const {
        const int component_id{Component<TComponent>::_get_id()};
        const int entity_id{entity.get_id()};
        auto pool{std::static_pointer_cast<Pool<TComponent>>(
            _component_pools[component_id])};
        return pool->get(entity_id);
    }

    ////////////////////////////////////////
    ////////////// SYSTEMS /////////////////
    ////////////////////////////////////////

    // add entity to systems where the component signature is set
    void add_entity_to_systems(Entity entity);

    template <typename TSystem, typename... TSystemArgs>
    inline void add_system(TSystemArgs &&...args) {
        std::shared_ptr<TSystem> new_system(
            std::make_shared<TSystem>(std::forward<TSystemArgs>(args)...));
        _systems.insert(
            std::make_pair(std::type_index(typeid(TSystem)), new_system));
    }

    template <typename TSystem>
    inline void remove_system() {
        const auto system{_systems.find(std::type_index(typeid(TSystem)))};
        if (system != _systems.end()) {
            _systems.erase(system);
        }
    }

    template <typename TSystem>
    inline bool has_system() const {
        const auto system{_systems.find(std::type_index(typeid(TSystem)))};
        return system != _systems.end();
    }

    template <typename TSystem>
    inline TSystem &get_system() const {
        const auto system{_systems.find(std::type_index(typeid(TSystem)))};
        return *(std::static_pointer_cast<TSystem>(system->second));
    }
};

////////////////////////////////////////
//////// ENTITY IMPLEMENTATION /////////
////////////////////////////////////////

template <typename TComponent, typename... TComponentArgs>
void Entity::add_component(TComponentArgs &&...args) {
    assert(registry);
    registry->add_component<TComponent>(*this,
                                        std::forward<TComponentArgs>(args)...);
}

template <typename TComponent>
void Entity::remove_component() {
    assert(registry);
    registry->remove_component<TComponent>(*this);
}

template <typename TComponent>
bool Entity::has_component() const {
    assert(registry);
    return registry->has_component<TComponent>(*this);
}

template <typename TComponent>
TComponent &Entity::get_component() const {
    assert(registry);
    return registry->get_component<TComponent>(*this);
}

}  // namespace debby::ecs

#endif  // DEBBY_SYSTEMS_ECS_H_
