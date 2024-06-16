#ifndef DEBBY_MANAGERS_EVENT_MANAGER_HPP_
#define DEBBY_MANAGERS_EVENT_MANAGER_HPP_

#include <functional>
#include <list>
#include <map>
#include <memory>
#include <typeindex>
#include <utility>

#include "../events/event.hpp"

namespace debby {

class IEventCallback {
   private:
    virtual void _call(Event &e) = 0;

   public:
    virtual ~IEventCallback() = default;

    inline void execute(Event &e) { _call(e); }
};

template <typename TOwner, typename TEvent>
class EventCallback : public IEventCallback {
   private:
    typedef void (TOwner::*CallbackFunction)(TEvent &);

    TOwner *_owner;
    CallbackFunction _callback;

    inline void _call(Event &e) override {
        std::invoke(_callback, _owner, static_cast<TEvent &>(e));
    }

   public:
    EventCallback(TOwner *owner, CallbackFunction callback)
        : _owner(owner), _callback(callback) {}

    ~EventCallback() override = default;
};

class EventManager {
    typedef std::list<std::unique_ptr<debby::IEventCallback>> HandlerList;
    using EventSubscribers =
        std::map<std::type_index, std::unique_ptr<HandlerList>>;

   private:
    static EventSubscribers _subscribers;

   public:
    // TODO probably rethink this and make a "disconnect" method instead
    inline static void reset() { _subscribers.clear(); }

    template <typename TEvent, typename TOwner>
    inline static void connect(TOwner *owner,
                               void (TOwner::*_callbackFunction)(TEvent &)) {
        if (!_subscribers[typeid(TEvent)].get()) {
            _subscribers[typeid(TEvent)] = std::make_unique<HandlerList>();
        }
        auto subscriber{std::make_unique<EventCallback<TOwner, TEvent>>(
            owner, _callbackFunction)};
        _subscribers[typeid(TEvent)]->push_back(std::move(subscriber));
    }

    template <typename TEvent, typename... TEventArgs>
    inline static void emit(TEventArgs &&...args) {
        auto handlers{_subscribers[typeid(TEvent)].get()};
        if (handlers) {
            for (auto iter = handlers->begin(); iter != handlers->end();
                 iter++) {
                auto handler{iter->get()};
                TEvent event(std::forward<TEventArgs>(args)...);
                handler->execute(event);
            }
        }
    }
};

}  // namespace debby

#endif  // DEBBY_MANAGERS_EVENT_MANAGER_HPP_
