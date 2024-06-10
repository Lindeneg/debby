#include <spdlog/common.h>
#include <spdlog/spdlog.h>

#include <cstdlib>

#include "./game_manager.h"

int main(int argc, char *argv[]) {
    spdlog::set_level(spdlog::level::debug);
    debby::GameManager game_manager{};
    if (!game_manager.initialize()) {
        return EXIT_FAILURE;
    }
    game_manager.run();
    game_manager.destroy();
    return EXIT_SUCCESS;
}
