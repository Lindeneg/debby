#ifndef DEBBY_GAME_H_
#define DEBBY_GAME_H_

namespace debby::game {
bool initialize(int w = 0, int h = 0) noexcept;
void run() noexcept;
void destroy() noexcept;

void process_input() noexcept;
void update() noexcept;
void render() noexcept;
}  // namespace debby::game

#endif  // DEBBY_GAME_H_
