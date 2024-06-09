#include <cstdlib>

#include "./game.h"
#include "./log.h"

int main(int argc, char *argv[]) {
    debby::log::set_level(debby::log::LOG_LEVEL_VERBOSE);
    if (!debby::game::initialize()) {
        return EXIT_FAILURE;
    }
    debby::game::run();
    debby::game::destroy();
    return EXIT_SUCCESS;
}

