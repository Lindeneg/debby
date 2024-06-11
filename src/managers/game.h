#ifndef DEBBY_MANAGERS_GAME_H_
#define DEBBY_MANAGERS_GAME_H_

#include <SDL2/SDL.h>

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

    ecs::Registry* _registry;

    double _delta_time;
    int _previous_frame_time;

    bool _initialize_sdl();
    void _cap_frame_rate();
    void _calculate_delta_time();
    void _set_render_draw_color(Color color);

   public:
    int window_width;
    int window_height;

    Game();
    ~Game();

    inline void cap_frame_rate() { _do_cap_frame_rate = true; }

    inline void uncap_frame_rate() { _do_cap_frame_rate = false; }

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
