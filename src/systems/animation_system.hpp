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
            auto &anim{entity.get_component<AnimationComponent>()};
            auto &active_anim{anim.get_active_animation()};
            auto &sprite{entity.get_component<SpriteComponent>()};

            if (anim.is_started()) {
                active_anim.current_frame =
                    ((SDL_GetTicks() - active_anim.start_time) *
                     active_anim.frame_rate / 1000) %
                    active_anim.num_frames;
            } else {
                active_anim.current_frame = 0;
            }

            sprite.src_rect.x = active_anim.current_frame * sprite.width;
            sprite.src_rect.y = active_anim.start_y * sprite.width;
        }
    }
};
}  // namespace debby
