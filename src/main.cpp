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
    debby::screen::draw_outlined_rect(glm::vec2(200, 200), glm::vec2(50, 50),
                                      debby::color::magenta);
    debby::screen::draw_filled_rect(glm::vec2(300, 300), glm::vec2(50, 50),
                                    debby::color::magenta);
    debby::screen::swap_buffers();
    debby::screen::render_present();
    debby::game::run();
    debby::game::destroy();
    return EXIT_SUCCESS;
}

