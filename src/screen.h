#ifndef DEBBY_SCREEN_H_
#define DEBBY_SCREEN_H_

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>

namespace debby::screen {
extern SDL_DisplayMode display_mode;
extern SDL_Window* window;
extern SDL_Renderer* renderer;

bool initialize(int w, int h) noexcept;
void destroy() noexcept;
}  // namespace debby::screen

#endif  // DEBBY_SCREEN_H_
