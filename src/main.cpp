#include <cstdlib>

#include "./game.h"

int main(int argc, char *argv[]) {
    Game game{};
    if (!game.initialize()) {
        // log error
        return EXIT_FAILURE;
    }
    game.run();
    game.destroy();
    return EXIT_SUCCESS;
}
