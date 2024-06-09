#include <cstdlib>

#include "./game_manager.h"
#include "./log.h"

int main(int argc, char *argv[]) {
    debby::log::set_level(debby::log::LOG_LEVEL_VERBOSE);
    debby::GameManager game_manager{};
    if (!game_manager.initialize()) {
        return EXIT_FAILURE;
    }
    game_manager.run();
    game_manager.destroy();
    return EXIT_SUCCESS;
}
