#include "screen_manager.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <spdlog/spdlog.h>

static uint32_t sdl_subsystem_flags{SDL_INIT_VIDEO | SDL_INIT_TIMER |
                                    SDL_INIT_EVENTS};

static SDL_Window *window{nullptr};
static SDL_Renderer *renderer{nullptr};
static SDL_DisplayMode display_mode{};
static glm::ivec2 dimensions{glm::ivec2(0, 0)};

static bool initialize_sdl() {
    if (SDL_WasInit(sdl_subsystem_flags)) {
        spdlog::error("SDL has already been initialized");
        return true;
    }
    if (SDL_Init(sdl_subsystem_flags) != 0) {
        spdlog::error("failed to initialize SDL {0}", SDL_GetError());
        return false;
    }
    if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) == 0) {
        spdlog::error("failed to initialize SDL Image {0}", SDL_GetError());
        return false;
    }
    return true;
}

bool debby::managers::screen::initialize() {
    if (!initialize_sdl()) {
        return false;
    }
    SDL_GetCurrentDisplayMode(0, &display_mode);
    dimensions.x = display_mode.w;
    dimensions.y = display_mode.h;
    if (!window) {
        window = SDL_CreateWindow("Debby", SDL_WINDOWPOS_CENTERED,
                                  SDL_WINDOWPOS_CENTERED, dimensions.x,
                                  dimensions.y, SDL_WINDOW_BORDERLESS);
        if (!window) {
            spdlog::error("failed to create SDL window {0}", SDL_GetError());
            return false;
        }
        spdlog::trace("initialized SDL window ({0:d},{1:d})", dimensions.x,
                      dimensions.y);
    }
    if (!renderer) {
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (!renderer) {
            spdlog::error("failed to create SDL renderer {0}", SDL_GetError());
            SDL_DestroyWindow(window);
            window = nullptr;
            return false;
        }
        spdlog::trace("initialized SDL renderer");
    }
    return true;
}

void debby::managers::screen::set_draw_color(Color color) {
    assert(renderer);
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

void debby::managers::screen::clear() {
    assert(renderer);
    set_draw_color(color::black);
    SDL_RenderClear(renderer);
}

void debby::managers::screen::present() {
    assert(renderer);
    SDL_RenderPresent(renderer);
}

glm::ivec2 debby::managers::screen::get_dimensions() { return dimensions; }

SDL_Window *debby::managers::screen::get_window() {
    assert(window);
    return window;
}

SDL_Renderer *debby::managers::screen::get_renderer() {
    assert(renderer);
    return renderer;
}

void debby::managers::screen::destroy() {
    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
        spdlog::trace("destroyed SDL renderer");
    }
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
        spdlog::trace("destroyed SDL window");
    }
    IMG_Quit();
    SDL_QuitSubSystem(sdl_subsystem_flags);
    SDL_Quit();
}

