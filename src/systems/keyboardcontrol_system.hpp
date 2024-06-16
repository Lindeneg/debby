#ifndef DEBBY_SYSTEMS_KEYBOARDCONTROL_SYSTEM_HPP_
#define DEBBY_SYSTEMS_KEYBOARDCONTROL_SYSTEM_HPP_

#include <spdlog/spdlog.h>

#include "../ecs/ecs.hpp"
#include "../events/keypressed_event.hpp"
#include "../managers/event_manager.hpp"

namespace debby {

class KeyboardControlSystem : public ecs::System {
   public:
    KeyboardControlSystem() {}

    inline void subscribe_to_events() {
        EventManager::connect<KeyPressedEvent>(
            this, &KeyboardControlSystem::on_key_pressed);
    }

    inline void on_key_pressed(KeyPressedEvent &event) {
        spdlog::debug("KeyPressed: ({0:d},{1})", event.symbol,
                      static_cast<char>(event.symbol));
    }

    inline void update() {}
};
}  // namespace debby

#endif  // DEBBY_SYSTEMS_KEYBOARDCONTROL_SYSTEM_HPP_
