#pragma once

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

