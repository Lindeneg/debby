#include "game_manager.h"

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_image.h>

#include "./globals.h"
#include "./log.h"
#include "./utils.h"

uint32_t debby::GameManager::_sdl_subsystem_flags{
    SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_EVENTS};

debby::GameManager::GameManager()
    : _is_running(false),
      _window(nullptr),
      _renderer(nullptr),
      _display_mode({}),
      _event({}),
      _delta_time(0),
      _previous_frame_time(0) {}

debby::GameManager::~GameManager() { destroy(); }

bool debby::GameManager::_initialize_sdl() {
    if (SDL_WasInit(_sdl_subsystem_flags)) {
        log::error("SDL has already been initialized");
        return true;
    }
    if (SDL_Init(_sdl_subsystem_flags) != 0) {
        log::error("failed to initialize SDL %s", SDL_GetError());
        return false;
    }
    if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) == 0) {
        log::error("failed to initialize SDL Image %s", SDL_GetError());
        return false;
    }
    return true;
}

void debby::GameManager::_respect_frame_target() {
    int time_to_wait =
        constants::FRAME_TARGET - (SDL_GetTicks() - _previous_frame_time);
    // only delay if too fast
    if (time_to_wait > 0 && time_to_wait < constants::FRAME_TARGET) {
        SDL_Delay(time_to_wait);
    }
    // calculate delta time
    _delta_time = (SDL_GetTicks() - _previous_frame_time) / 1000.0f;
    // clamp value (if running in debugger dt will be messed up)
    _delta_time = (utils::greater(_delta_time, constants::MAXIMUM_DT))
                      ? constants::MAXIMUM_DT
                      : _delta_time;
    // update previous frame time
    _previous_frame_time = SDL_GetTicks();
}

void debby::GameManager::_set_render_draw_color(Color color) {
    assert(_renderer);
    SDL_SetRenderDrawColor(_renderer, color.r, color.g, color.b, color.a);
}

bool debby::GameManager::initialize() {
    if (!_initialize_sdl()) {
        return false;
    }
    SDL_GetCurrentDisplayMode(0, &_display_mode);
    window_width = _display_mode.w;
    window_height = _display_mode.h;
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

glm::vec2 player_pos{};
glm::vec2 player_velocity{};

void debby::GameManager::setup() {
    player_pos = {10, 20};
    player_velocity = {0.5, 0};
}

void debby::GameManager::run() {
    setup();
    while (_is_running) {
        process_input();
        update();
        render();
    }
}

void debby::GameManager::process_input() {
    while (SDL_PollEvent(&_event)) {
        switch (_event.type) {
            case SDL_QUIT:
                _is_running = false;
                break;
            case SDL_KEYDOWN:
                if (_event.key.keysym.sym == SDLK_ESCAPE) {
                    _is_running = false;
                }
                break;
        }
    }
}

void debby::GameManager::update() {
    _respect_frame_target();
    // update position
    player_pos += player_velocity;
}

void debby::GameManager::render() {
    _set_render_draw_color(color::black);
    SDL_RenderClear(_renderer);

    auto texture{
        IMG_LoadTexture(_renderer, "./assets/Characters/Champions/Zhinja.png")};

    SDL_Rect src_rect{0, 0, 16, 16};
    SDL_Rect dst_rect{static_cast<int>(player_pos.x),
                      static_cast<int>(player_pos.y), 32, 32};
    SDL_RenderCopy(_renderer, texture, &src_rect, &dst_rect);

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
    SDL_QuitSubSystem(_sdl_subsystem_flags);
    SDL_Quit();
}
