#include "screen.h"

#include <SDL2/SDL.h>

#include "./log.h"

static uint32_t sdl_subsystems{SDL_INIT_VIDEO | SDL_INIT_TIMER |
                               SDL_INIT_EVENTS};

static bool initialize_sdl() {
    if (SDL_WasInit(sdl_subsystems)) {
        debby::log::error("SDL has already been initialized\n");
        return true;
    }
    if (SDL_Init(sdl_subsystems) != 0) {
        debby::log::error("failed to initialize SDL %s\n", SDL_GetError());
        return false;
    }
    // TODO initialize IMG and TTF
    return true;
}

SDL_DisplayMode debby::screen::display_mode{SDL_DisplayMode()};
SDL_Window* debby::screen::window{nullptr};
SDL_Renderer* debby::screen::renderer{nullptr};

bool debby::screen::initialize(int w, int h) noexcept {
    if (!initialize_sdl()) {
        return false;
    }
    SDL_GetCurrentDisplayMode(0, &display_mode);
    if (!window) {
        if (!w) {
            w = display_mode.w;
        }
        if (!h) {
            h = display_mode.h;
        }
        window = SDL_CreateWindow("Debby", SDL_WINDOWPOS_CENTERED,
                                  SDL_WINDOWPOS_CENTERED, w, h,
                                  SDL_WINDOW_BORDERLESS);
        if (!window) {
            debby::log::error("failed to create SDL window %s\n",
                              SDL_GetError());
            return false;
        }
        debby::log::info("initialized SDL window (%d,%d)\n", w, h);
    }
    if (!renderer) {
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (!renderer) {
            debby::log::error("failed to create SDL renderer %s\n",
                              SDL_GetError());
            SDL_DestroyWindow(window);
            window = nullptr;
            return false;
        }
        debby::log::info("initialized SDL renderer\n");
    }
    return true;
}

void debby::screen::destroy() noexcept {
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
        debby::log::verbose("destroyed SDL window\n");
    }
    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
        debby::log::verbose("destroyed SDL renderer\n");
    }
    debby::log::verbose("quiting all SDL systems\n");
    SDL_QuitSubSystem(sdl_subsystems);
    SDL_Quit();
}

