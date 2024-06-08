#include <cstdlib>

#include "./game.h"
#include "./log.h"
#include "globals.h"
#include "screen.h"

int main(int argc, char *argv[]) {
    debby::log::set_level(debby::log::LOG_LEVEL_VERBOSE);
    if (!debby::game::initialize()) {
        return EXIT_FAILURE;
    }
    debby::screen::draw_line(glm::vec2(0, 0), glm::vec2(100, 100),
                             debby::color::red);
    debby::screen::swap_buffers();
    debby::screen::render_present();
    debby::game::run();
    debby::game::destroy();
    return EXIT_SUCCESS;
}

