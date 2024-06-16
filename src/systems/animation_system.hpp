#ifndef DEBBY_SYSTEMS_ANIMATION_SYSTEM_HPP_
#define DEBBY_SYSTEMS_ANIMATION_SYSTEM_HPP_

#include "../components/animation_component.hpp"
#include "../components/sprite_component.hpp"
#include "../ecs/ecs.hpp"

namespace debby {

/* AnimationSystem handles Entity animations
 *
 * Entities must have the following components:
 * - SpriteComponent
 * - AnimationComponent */
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

            // TODO probably tidy this up a bit
            if (anim.is_started()) {
                auto new_frame{
                    static_cast<int>((SDL_GetTicks() - active_anim.start_time) *
                                     active_anim.frame_rate / 1000)};
                if (new_frame > active_anim.num_frames && !active_anim.loop) {
                    active_anim.current_frame = 0;
                    anim.stop();
                } else {
                    active_anim.current_frame =
                        new_frame % active_anim.num_frames;
                }

            } else {
                active_anim.current_frame = 0;
            }

            sprite.src_x = active_anim.current_frame * sprite.width;
            sprite.src_y = active_anim.start_y * sprite.width;
        }
    }
};
}  // namespace debby

#endif  // DEBBY_SYSTEMS_ANIMATION_SYSTEM_HPP_
