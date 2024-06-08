#include "game.h"

#include "./log.h"
#include "./screen.h"

bool debby::game::initialize(int w, int h) noexcept {
    if (!screen::initialize(w, h)) {
        debby::log::error("failed to initialize screen\n");
        return false;
    }
    return true;
}

void debby::game::run() noexcept {
    while (true) {
        process_input();
        update();
        render();
    }
}

void debby::game::process_input() noexcept {}

void debby::game::update() noexcept {}

void debby::game::render() noexcept {}

void debby::game::destroy() noexcept { screen::destroy(); }
