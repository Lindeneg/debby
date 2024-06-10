#ifndef DEBBY_SYSTEMS_ECS_H_
#define DEBBY_SYSTEMS_ECS_H_

#include <bitset>
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

template <typename T>
class Component : public IComponent {
    friend class System;

   private:
    inline static int _get_id() {
        static auto id{_next_id++};
        return id;
    }
};

////////////////////////////////////////
///////// ENTITY DEFINITION ////////////
////////////////////////////////////////

/*
 * Entity is a game object that can contain
 * a collection of different components
 * */
class Entity {
   private:
    int _id;

   public:
    Entity(int id);

    int get_id() const;
};

////////////////////////////////////////
////////// SYSTEM DEFINITION ///////////
////////////////////////////////////////

/*
 * System processes entities that contain a specific signature
 * */
class System {
   private:
    ComponentSignature _signature;
    std::vector<Entity> _entities;

   public:
    System();
    ~System();

    const ComponentSignature& get_signature() const;

    const std::vector<Entity>& get_entities() const;

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
///////// REGISTRY DEFINITION //////////
////////////////////////////////////////

/*
 * Registry
 * */
class Registry {};
}  // namespace debby::ecs

#endif  // DEBBY_SYSTEMS_ECS_H_
