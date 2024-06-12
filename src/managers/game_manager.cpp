#include "game_manager.hpp"

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_timer.h>
#include <spdlog/spdlog.h>

#include <memory>

#include "../common/utils.hpp"
#include "../components/rigidbody_component.hpp"
#include "../components/sprite_component.hpp"
#include "../components/transform_component.hpp"
#include "../ecs/ecs.hpp"
#include "../systems/movement_system.hpp"
#include "../systems/render_system.hpp"
#include "./asset_manager.hpp"
#include "./screen_manager.hpp"

static bool is_running{false};
static bool do_cap_frame_rate{false};
static SDL_Event event{};
static float delta_time{};
static float previous_frame_time{};

static std::unique_ptr<debby::ecs::Registry> registry{
    std::make_unique<debby::ecs::Registry>()};

static void cap_frame_rate() {
    int time_to_wait = static_cast<int>(
        debby::constants::FRAME_TARGET -
        (static_cast<float>(SDL_GetTicks()) - previous_frame_time));
    // only delay if too fast
    if (time_to_wait > 0 && time_to_wait < debby::constants::FRAME_TARGET) {
        SDL_Delay(time_to_wait);
    }
}

static void calculate_delta_time() {
    if (do_cap_frame_rate) {
        cap_frame_rate();
    }
    // calculate delta time
    delta_time =
        (static_cast<float>(SDL_GetTicks()) - previous_frame_time) / 1000.f;
    // clamp value (if running in debugger dt will be messed up)
    delta_time =
        (debby::utils::greater(delta_time, debby::constants::MAXIMUM_DT))
            ? debby::constants::MAXIMUM_DT
            : delta_time;
    // update previous frame time
    previous_frame_time = static_cast<float>(SDL_GetTicks());
}

bool debby::managers::game::initialize() {
    if (!screen::initialize()) {
        return false;
    }
    is_running = true;
    return true;
}

void debby::managers::game::setup() {
    registry->add_system<MovementSystem>();
    registry->add_system<RenderSystem>();
}

void debby::managers::game::load_level(int level) {
    asset::add_texture("zhinja", "./assets/Characters/Champions/Zhinja.png");
    asset::add_texture("grum", "./assets/Characters/Champions/Grum.png");

    ecs::Entity zhinja{registry->create_entity()};

    zhinja.add_component<TransformComponent>(glm::vec2(10.f, 30.f),
                                             glm::vec2(3.f, 3.f));
    zhinja.add_component<RigidBodyComponent>(glm::vec2(10.f, 15.f));
    zhinja.add_component<SpriteComponent>("zhinja", 16, 16);

    ecs::Entity grum{registry->create_entity()};

    grum.add_component<TransformComponent>(glm::vec2(40.f, 50.f),
                                           glm::vec2(3.f, 3.f));
    grum.add_component<RigidBodyComponent>(glm::vec2(50.f, 15.f));
    grum.add_component<SpriteComponent>("grum", 16, 16);
}

void debby::managers::game::run() {
    setup();
    while (is_running) {
        process_input();
        update();
        render();
    }
}

void debby::managers::game::process_input() {
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                is_running = false;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    is_running = false;
                }
                break;
        }
    }
}

void debby::managers::game::update() {
    calculate_delta_time();
    registry->get_system<MovementSystem>().update(delta_time);
    registry->update();
}

void debby::managers::game::render() {
    screen::clear();

    registry->get_system<RenderSystem>().update();

    screen::present();
}

void debby::managers::game::destroy() {
    asset::destroy();
    screen::destroy();
}

