#include <spdlog/spdlog.h>

#include <cstdlib>

#include "./managers/game.h"

int main(int argc, char *argv[]) {
    spdlog::set_level(spdlog::level::debug);
    debby::manager::Game game_manager{};
    if (!game_manager.initialize()) {
        return EXIT_FAILURE;
    }
    game_manager.run();
    game_manager.destroy();
    return EXIT_SUCCESS;
}
