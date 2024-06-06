#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <glm/glm.hpp>
#include <imgui/imgui.h>
#include <sol/sol.hpp>

int main(int argc, char *argv[]) {
    sol::state lua;
    lua.open_libraries(sol::lib::base);
    SDL_Init(SDL_INIT_EVERYTHING);
    std::cout << "Hello world!\n";
    return 0;
}
