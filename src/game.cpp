#include "game.h"

#include <SDL2/SDL_events.h>

#include "./globals.h"
#include "./screen.h"

static debby::Context context{};
static SDL_Event event{};
static bool is_running{false};

bool debby::game::initialize() noexcept {
    if (!screen::initialize(context)) {
        return false;
    }
    return true;
}

void debby::game::run() noexcept {
    is_running = true;
    while (is_running) {
        process_input();
        update();
        render();
    }
}

void debby::game::process_input() noexcept {
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                is_running = false;
                break;
            case SDL_KEYDOWN: {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    is_running = false;
                    break;
                }
            }
            default: {
                break;
            }
        }
        // TODO handle game object input handlers
    }
}

void debby::game::update() noexcept {}

void debby::game::render() noexcept {}

void debby::game::destroy() noexcept { screen::destroy(); }
