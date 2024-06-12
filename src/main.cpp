#if defined(_WIN32) || defined(WIN32)
#define SDL_MAIN_HANDLED
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <spdlog/spdlog.h>

#include <cstdlib>

#include "./managers/game_manager.hpp"

int main(int argc, char *argv[]) {
    spdlog::set_level(spdlog::level::trace);
    debby::manager::GameManager game_manager{};
    if (!game_manager.initialize()) {
        return EXIT_FAILURE;
    }
    game_manager.run();
    game_manager.destroy();
    return EXIT_SUCCESS;
}
