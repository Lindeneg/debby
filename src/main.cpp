#if defined(_MSC_VER)
#define SDL_MAIN_HANDLED
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <spdlog/spdlog.h>

#include <cstdlib>

#include "./managers/game_manager.hpp"

int main(int argc, char *argv[]) {
    spdlog::set_level(spdlog::level::trace);
    if (!debby::managers::game::initialize()) {
        return EXIT_FAILURE;
    }
    debby::managers::game::run();
    debby::managers::game::destroy();
    return EXIT_SUCCESS;
}

