#ifndef DEBBY_MANAGERS_GAME_H_
#define DEBBY_MANAGERS_GAME_H_

#include <SDL2/SDL.h>

#include <glm/ext/vector_int2.hpp>
#include <memory>

#include "../common/globals.h"
#include "../systems/ecs.h"

namespace debby::manager {

////////////////////////////////////////
/////// GAME MANAGER DEFINITION ////////
////////////////////////////////////////

/*
 * Game is responsible for initializing SDL,
 * setting up game data, running the game-loop,
 * and updating/rendering active entities */
class Game {
   private:
    // SDL subsystems to initialize
    static uint32_t _sdl_subsystem_flags;

    bool _is_running;
    bool _do_cap_frame_rate;

    SDL_Window* _window;
    SDL_Renderer* _renderer;

    SDL_DisplayMode _display_mode;
    SDL_Event _event;

    std::unique_ptr<ecs::Registry> _registry;

    float _delta_time;
    int _previous_frame_time;

    int _window_width;
    int _window_height;

    bool _initialize_sdl();
    void _cap_frame_rate();
    void _calculate_delta_time();
    void _set_render_draw_color(Color color);

   public:
    Game();
    ~Game();

    inline void cap_frame_rate() { _do_cap_frame_rate = true; }
    inline void uncap_frame_rate() { _do_cap_frame_rate = false; }
    inline int get_window_width() const { return _window_width; }
    inline int get_window_height() const { return _window_height; }
    inline glm::ivec2 get_window_vec() const {
        return glm::ivec2(_window_width, _window_height);
    }

    bool initialize();
    void setup();
    void run();
    void process_input();
    void update();
    void render();
    void destroy();
};
}  // namespace debby::manager

#endif  // DEBBY_MANAGERS_GAME_H_
