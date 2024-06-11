#include "game.h"

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_image.h>
#include <spdlog/spdlog.h>

#include <memory>

#include "../common/globals.h"
#include "../common/utils.h"

////////////////////////////////////////
///// GAME MANAGER IMPLEMENTATION //////
////////////////////////////////////////

uint32_t debby::manager::Game::_sdl_subsystem_flags{
    SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_EVENTS};

debby::manager::Game::Game()
    : _is_running(false),
      _do_cap_frame_rate(false),
      _window(nullptr),
      _renderer(nullptr),
      _display_mode({}),
      _event({}),
      _registry(std::make_unique<ecs::Registry>()),
      _delta_time(0),
      _previous_frame_time(0),
      _window_width(0),
      _window_height(0) {}

debby::manager::Game::~Game() { destroy(); }

bool debby::manager::Game::_initialize_sdl() {
    if (SDL_WasInit(_sdl_subsystem_flags)) {
        spdlog::error("SDL has already been initialized");
        return true;
    }
    if (SDL_Init(_sdl_subsystem_flags) != 0) {
        spdlog::error("failed to initialize SDL {0}", SDL_GetError());
        return false;
    }
    if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) == 0) {
        spdlog::error("failed to initialize SDL Image {0}", SDL_GetError());
        return false;
    }
    return true;
}

void debby::manager::Game::_cap_frame_rate() {
    int time_to_wait =
        constants::FRAME_TARGET - (SDL_GetTicks() - _previous_frame_time);
    // only delay if too fast
    if (time_to_wait > 0 && time_to_wait < constants::FRAME_TARGET) {
        SDL_Delay(time_to_wait);
    }
}

void debby::manager::Game::_calculate_delta_time() {
    if (_do_cap_frame_rate) {
        _cap_frame_rate();
    }
    // calculate delta time
    _delta_time =
        static_cast<double>(SDL_GetTicks() - _previous_frame_time) / 1000.0;
    // clamp value (if running in debugger dt will be messed up)
    _delta_time = (utils::greater(_delta_time, constants::MAXIMUM_DT))
                      ? constants::MAXIMUM_DT
                      : _delta_time;
    // update previous frame time
    _previous_frame_time = SDL_GetTicks();
}

void debby::manager::Game::_set_render_draw_color(Color color) {
    assert(_renderer);
    SDL_SetRenderDrawColor(_renderer, color.r, color.g, color.b, color.a);
}

bool debby::manager::Game::initialize() {
    if (!_initialize_sdl()) {
        return false;
    }
    SDL_GetCurrentDisplayMode(0, &_display_mode);
    _window_width = _display_mode.w;
    _window_height = _display_mode.h;
    if (!_window) {
        _window = SDL_CreateWindow("Debby", SDL_WINDOWPOS_CENTERED,
                                   SDL_WINDOWPOS_CENTERED, _window_width,
                                   _window_height, SDL_WINDOW_BORDERLESS);
        if (!_window) {
            spdlog::error("failed to create SDL window {0}", SDL_GetError());
            return false;
        }
        spdlog::debug("initialized SDL window ({0:d},{1:d})", _window_width,
                      _window_height);
    }
    if (!_renderer) {
        _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
        if (!_renderer) {
            spdlog::error("failed to create SDL renderer {0}", SDL_GetError());
            SDL_DestroyWindow(_window);
            _window = nullptr;
            return false;
        }
        spdlog::debug("initialized SDL renderer");
    }
    SDL_SetWindowFullscreen(_window, SDL_WINDOW_FULLSCREEN);
    _is_running = true;
    return true;
}

void debby::manager::Game::setup() {
    ecs::Entity zhinja{_registry->create_entity()};
    ecs::Entity zhinja2{_registry->create_entity()};
}

void debby::manager::Game::run() {
    setup();
    while (_is_running) {
        process_input();
        update();
        render();
    }
}

void debby::manager::Game::process_input() {
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

void debby::manager::Game::update() { _calculate_delta_time(); }

void debby::manager::Game::render() {
    _set_render_draw_color(color::black);
    SDL_RenderClear(_renderer);

    //    auto texture{
    //        IMG_LoadTexture(_renderer,
    //        "./assets/Characters/Champions/Zhinja.png")};
    //
    //    SDL_Rect src_rect{0, 0, 16, 16};
    //    SDL_Rect dst_rect{static_cast<int>(player_pos.x),
    //                      static_cast<int>(player_pos.y), 32, 32};
    //    SDL_RenderCopy(_renderer, texture, &src_rect, &dst_rect);

    SDL_RenderPresent(_renderer);
}

void debby::manager::Game::destroy() {
    if (_renderer) {
        SDL_DestroyRenderer(_renderer);
        _renderer = nullptr;
        spdlog::debug("destroyed SDL renderer");
    }
    if (_window) {
        SDL_DestroyWindow(_window);
        _window = nullptr;
        spdlog::debug("destroyed SDL window");
    }
    IMG_Quit();
    SDL_QuitSubSystem(_sdl_subsystem_flags);
    SDL_Quit();
}
