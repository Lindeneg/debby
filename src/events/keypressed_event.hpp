#ifndef DEBBY_EVENTS_KEYPRESSED_EVENT_HPP_
#define DEBBY_EVENTS_KEYPRESSED_EVENT_HPP_

#include <SDL2/SDL.h>

#include "./event.hpp"

namespace debby {
class KeyPressedEvent : public Event {
   public:
    SDL_Keycode symbol;

    KeyPressedEvent(SDL_Keycode symbol) : symbol(symbol) {}
};
}  // namespace debby

#endif  // DEBBY_EVENTS_KEYPRESSED_EVENT_HPP_
