#ifndef DEBBY_ECS_ECS_HPP_
#define DEBBY_ECS_ECS_HPP_

#include <spdlog/spdlog.h>

#include <atomic>
#include <bitset>
#include <deque>
#include <memory>
#include <set>
#include <typeindex>
#include <unordered_map>
#include <utility>
#include <vector>

namespace debby::ecs {
/* Universal ID type */
using Id = unsigned int;

/* Thread-safe ID counter */
using IdCounter = std::atomic<Id>;

/* Max number of components an entity can have */
constexpr unsigned int MAX_COMPONENTS{32};

/* Describes which component(s) are enabled on an entity */
typedef std::bitset<MAX_COMPONENTS> ComponentSignature;

/*
 * IComponent is a simple wrapper to hold an ID counter */
class IComponent {
   protected:
    static IdCounter _next_id;
};

/*
 * Component is an abstract class instantiated
 * for once for each unique component subtype */
template <typename>
class Component : public IComponent {
   public:
    inline static Id get_id() {
        /* Since a unique component class is made for each type
         * static id variable will only be created once per instance */
        static IdCounter id{_next_id++};
        return id.load();
    }
};

/*
 * Entity is effectively just an identifier used by a system
 * and registry to register components and process behavior */
class Entity {
   private:
    Id _id;

   public:
    class Registry *registry;

    explicit Entity(Id id);
    ~Entity() = default;

    [[nodiscard]] Id get_id() const;
    void kill();

    bool operator==(const Entity &other) const;
    bool operator!=(const Entity &other) const;
    bool operator<(const Entity &other) const;
    bool operator>(const Entity &other) const;

    template <typename TComponent, typename... TComponentArgs>
    TComponent &add_component(TComponentArgs &&...args);

    template <typename TComponent>
    void remove_component();

    template <typename TComponent>
    [[nodiscard]] bool has_component() const;

    template <typename TComponent>
    TComponent &get_component() const;
};

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

    [[nodiscard]] const ComponentSignature &get_signature() const;

    [[nodiscard]] const std::vector<Entity> &get_entities() const;

    void add_entity(Entity entity);

    void remove_entity(Entity entity);

    /* Defines the component that an entity must have
     * in order to be considered by the system */
    template <typename TComponent>
    inline void require_component() {
        const Id component_id{Component<TComponent>::get_id()};
        _signature.set(component_id);
    }
};

/*
 * IPool is just a simple interface that can be used
 * when the T used in Pool is not known precisely */
class IPool {
   public:
    virtual ~IPool() = default;
};

/*
 * Pool is a vector wrapper of type T objects */
template <typename T>
class Pool : public IPool {
   private:
    std::vector<T> _data;

   public:
    explicit Pool(unsigned int size = 100) { _data.resize(size); }

    ~Pool() override = default;

    [[nodiscard]] inline bool is_empty() const { return _data.empty(); }

    [[nodiscard]] inline unsigned int get_size() const {
        return static_cast<int>(_data.size());
    }

    inline void set_size(unsigned int new_size) { _data.resize(new_size); }

    inline void flush() { _data.clear(); }

    inline void append(T obj) { _data.push_back(obj); }

    inline void set_item(unsigned int index, T object) {
        _data[index] = object;
    }

    inline T &get_item(unsigned int index) { return _data[index]; }

    inline T &operator[](unsigned int index) { return _data[index]; }
};

/*
 * Registry manages creation and destruction of entities,
 * adding systems and adding components to entities
 * */
class Registry {
   private:
    IdCounter _entity_counter;

    /* Each pool contains all data for a certain component type.
     * Vector index is component id and pool index is entity id */
    std::vector<std::shared_ptr<IPool>> _component_pools;

    /* Vector of component signatures per entity.
     * Vector index is equal to entity id */
    std::vector<ComponentSignature> _entity_component_signatures;

    std::unordered_map<std::type_index, std::shared_ptr<System>> _systems;

    /* Save entities to add/remove, such that they can
     * be processed in bulk at the end of each frame */
    std::set<Entity> _entities_add_queue;
    std::set<Entity> _entities_remove_queue;

    /* Saves the Id of a destroyed entity such that
     * it can be reused for other new entities */
    std::deque<Id> _free_ids;

    /* Add entity to systems where the component signature is set */
    void _add_entity_to_systems(Entity entity);

    /* Remove entity from systems */
    void _remove_entity_from_systems(Entity entity);

   public:
    Registry();
    ~Registry() = default;

    void update();

    Entity create_entity();
    void destroy_entity(Entity entity);

    template <typename TComponent, typename... TComponentArgs>
    inline TComponent &add_component(Entity entity, TComponentArgs &&...args) {
        const Id component_id{Component<TComponent>::get_id()};
        if (component_id >=
            static_cast<unsigned int>(_component_pools.size())) {
            /* Resize by one, since the resizing should be rare
             * so using the default vector resize could be expensive */
            _component_pools.resize(component_id + 1, nullptr);
        }
        if (!_component_pools[component_id]) {
            _component_pools[component_id] =
                std::make_shared<Pool<TComponent>>();
        }
        std::shared_ptr<Pool<TComponent>> component_pool{
            std::static_pointer_cast<Pool<TComponent>>(
                _component_pools[component_id])};
        const Id entity_id{entity.get_id()};
        if (entity_id >= component_pool->get_size()) {
            component_pool->set_size(_entity_counter.load());
        }

        spdlog::trace("adding {0} to entity {1:d}", typeid(TComponent).name(),
                      entity_id);

        TComponent new_component(std::forward<TComponentArgs>(args)...);
        component_pool->set_item(entity_id, new_component);
        _entity_component_signatures[entity_id].set(component_id);
        return component_pool->get_item(entity_id);
    }

    template <typename TComponent>
    inline void remove_component(Entity entity) {
        const Id component_id{Component<TComponent>::get_id()};
        const Id entity_id{entity.get_id()};

        spdlog::trace("removing {0} from entity {1:d}",
                      typeid(TComponent).name(), entity_id);

        _entity_component_signatures[entity_id].set(component_id, false);
    }

    template <typename TComponent>
    inline bool has_component(Entity entity) const {
        const Id component_id{Component<TComponent>::get_id()};
        const Id entity_id{entity.get_id()};
        return _entity_component_signatures[entity_id].test(component_id);
    }

    template <typename TComponent>
    inline TComponent &get_component(Entity entity) const {
        const Id component_id{Component<TComponent>::get_id()};
        const Id entity_id{entity.get_id()};
        auto pool{std::static_pointer_cast<Pool<TComponent>>(
            _component_pools[component_id])};
        return pool->get_item(entity_id);
    }

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
        if (system == _systems.end()) {
            spdlog::warn("tried to remove non-existent {0} from registry",
                         typeid(TSystem).name());
            return;
        }
        spdlog::debug("removing {0} from registry", system->first.name());
        _systems.erase(system);
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

template <typename TComponent, typename... TComponentArgs>
TComponent &Entity::add_component(TComponentArgs &&...args) {
    assert(registry);
    return registry->add_component<TComponent>(
        *this, std::forward<TComponentArgs>(args)...);
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

#endif  // DEBBY_ECS_ECS_HPP_
