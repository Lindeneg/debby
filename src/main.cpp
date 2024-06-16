#include <spdlog/spdlog.h>

#include <cstdlib>

#include "./managers/game_manager.hpp"

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
    spdlog::set_level(spdlog::level::debug);
    if (!debby::managers::game::initialize()) {
        return EXIT_FAILURE;
    }
    debby::managers::game::run();
    debby::managers::game::destroy();
    return EXIT_SUCCESS;
}
