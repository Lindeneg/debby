#include "game-manager.h"

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_image.h>

#include "./globals.h"
#include "./log.h"

static uint32_t sdl_subsystems{SDL_INIT_VIDEO | SDL_INIT_TIMER |
                               SDL_INIT_EVENTS};
static SDL_DisplayMode display_mode{};
static SDL_Event event{};

static bool initialize_sdl() {
    if (SDL_WasInit(sdl_subsystems)) {
        debby::log::error("SDL has already been initialized");
        return true;
    }
    if (SDL_Init(sdl_subsystems) != 0) {
        debby::log::error("failed to initialize SDL %s", SDL_GetError());
        return false;
    }
    if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) == 0) {
        debby::log::error("failed to initialize SDL Image %s", SDL_GetError());
        return false;
    }
    return true;
}

debby::GameManager::GameManager()
    : _is_running(false), _window(nullptr), _renderer(nullptr) {}

debby::GameManager::~GameManager() { destroy(); }

void debby::GameManager::set_render_draw_color(Color color) {
    assert(_renderer);
    SDL_SetRenderDrawColor(_renderer, color.r, color.g, color.b, color.a);
}

bool debby::GameManager::initialize() {
    if (!initialize_sdl()) {
        return false;
    }
    SDL_GetCurrentDisplayMode(0, &display_mode);
    window_width = display_mode.w;
    window_height = display_mode.h;
    if (!_window) {
        _window = SDL_CreateWindow("Debby", SDL_WINDOWPOS_CENTERED,
                                   SDL_WINDOWPOS_CENTERED, window_width,
                                   window_height, SDL_WINDOW_BORDERLESS);
        if (!_window) {
            log::error("failed to create SDL window %s", SDL_GetError());
            return false;
        }
        log::verbose("initialized SDL window (%d,%d)", window_width,
                     window_height);
    }
    if (!_renderer) {
        _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
        if (!_renderer) {
            log::error("failed to create SDL renderer %s", SDL_GetError());
            SDL_DestroyWindow(_window);
            _window = nullptr;
            return false;
        }
        log::verbose("initialized SDL renderer");
    }
    SDL_SetWindowFullscreen(_window, SDL_WINDOW_FULLSCREEN);
    _is_running = true;
    return true;
}

void debby::GameManager::setup() {}

void debby::GameManager::run() {
    setup();
    while (_is_running) {
        process_input();
        update();
        render();
    }
}

void debby::GameManager::process_input() {
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                _is_running = false;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    _is_running = false;
                }
                break;
        }
    }
}

void debby::GameManager::update() {
    // TODO: Update game objects...
}

void debby::GameManager::render() {
    set_render_draw_color(color::dark_gray);
    SDL_RenderClear(_renderer);

    // Draw a rectangle
    set_render_draw_color(color::red);
    SDL_Rect player = {10, 10, 20, 20};
    SDL_RenderFillRect(_renderer, &player);

    SDL_RenderPresent(_renderer);
}

void debby::GameManager::destroy() {
    if (_renderer) {
        SDL_DestroyRenderer(_renderer);
        _renderer = nullptr;
        log::verbose("destroyed SDL renderer");
    }
    if (_window) {
        SDL_DestroyWindow(_window);
        _window = nullptr;
        log::verbose("destroyed SDL window");
    }
    log::verbose("quitting all SDL systems");
    SDL_QuitSubSystem(sdl_subsystems);
    SDL_Quit();
}
