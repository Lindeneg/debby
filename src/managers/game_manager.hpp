#ifndef DEBBY_MANAGERS_GAME_MANAGER_HPP_
#define DEBBY_MANAGERS_GAME_MANAGER_HPP_

namespace debby::managers::game {

bool initialize();

void set_cap_frame_rate(bool state);

void setup();
void load_level(int level);
void run();

void process_input();
void update();
void render();

void destroy();

}  // namespace debby::managers::game

#endif  // DEBBY_MANAGERS_GAME_MANAGER_HPP_

