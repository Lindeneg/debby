#ifndef DEBBY_GAME_MANAGER_H_
#define DEBBY_GAME_MANAGER_H_

#include <SDL2/SDL.h>

#include "./globals.h"

namespace debby {

class GameManager {
   private:
    bool _is_running;
    SDL_Window* _window;
    SDL_Renderer* _renderer;

    void set_render_draw_color(Color color);

   public:
    int window_width;
    int window_height;

    GameManager();
    ~GameManager();

    bool initialize();
    void setup();
    void run();
    void process_input();
    void update();
    void render();
    void destroy();
};
}  // namespace debby

#endif  // DEBBY_GAME_MANAGER_H_
