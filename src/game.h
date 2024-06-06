#ifndef DEBBY_GAME_H_
#define DEBBY_GAME_H_

class Game {
   public:
    Game();
    ~Game();

    bool initialize();
    void run();
    void destroy();

    void process_input();
    void update();
    void render();
};

#endif  // DEBBY_GAME_H_
