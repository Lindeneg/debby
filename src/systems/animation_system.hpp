#pragma once

#include "../components/animation_component.hpp"
#include "../components/sprite_component.hpp"
#include "../ecs/ecs.hpp"

namespace debby {

class AnimationSystem : public ecs::System {
   public:
    AnimationSystem() {
        require_component<SpriteComponent>();
        require_component<AnimationComponent>();
    }

    inline void update() {
        for (auto entity : get_entities()) {
            auto &anim{entity.get_component<AnimationComponent>()
                           .get_active_animation()};
            auto &sprite{entity.get_component<SpriteComponent>()};

            anim.current_frame =
                ((SDL_GetTicks() - anim.start_time) * anim.frame_rate / 1000) %
                anim.num_frames;

            sprite.src_rect.x = anim.current_frame * sprite.width;
            sprite.src_rect.y = anim.start_y * sprite.width;
        }
    }
};
}  // namespace debby
