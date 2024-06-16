#ifndef DEBBY_COMPONENTS_ANIMATION_COMPONENT_HPP_
#define DEBBY_COMPONENTS_ANIMATION_COMPONENT_HPP_

#include <SDL2/SDL_timer.h>
#include <spdlog/spdlog.h>

#include <map>
#include <string>
#include <utility>

namespace debby {

/* AnimationContext contains information
 * animations in a given sprite-sheet */
struct AnimationContext {
    int num_frames;
    int start_y;
    int current_frame;
    int frame_rate;
    bool loop;
    int start_time;

    explicit AnimationContext(int num_frames = 1, int start_y = 0,
                              int frame_rate = 1, bool loop = true) {
        this->num_frames = num_frames;
        this->start_y = start_y;
        this->current_frame = 1;
        this->frame_rate = frame_rate;
        this->loop = loop;
        this->start_time = static_cast<int>(SDL_GetTicks());
    }

    ~AnimationContext() = default;
};

/* AnimationComponent allows an entity the notion
 * of animations. It stores a collection of
 * AnimationContext's in a map with string keys */
class AnimationComponent {
   private:
    std::map<std::string, AnimationContext> _animations;
    std::string _active_animation;
    AnimationContext _default_context;
    bool _is_started;

   public:
    AnimationComponent()
        : _animations({}),
          _active_animation({}),
          _default_context({}),
          _is_started(false) {}

    ~AnimationComponent() = default;

    [[nodiscard]] inline bool is_started() const { return _is_started; }

    inline void start() { _is_started = true; }

    inline void stop() { _is_started = false; }

    inline AnimationContext &get_active_animation() {
        auto iter{_animations.find(_active_animation)};
        if (iter == _animations.end()) {
            spdlog::warn("no active animations found, returning default");
            return _default_context;
        }
        return iter->second;
    }

    inline void add_animation(const std::string &name, AnimationContext ctx) {
        _animations.insert(std::make_pair(name, ctx));
        if (_animations.size() == 1) {
            spdlog::debug("setting default animation to '{0}'", name);
            _active_animation = name;
        }
    }

    inline void set_active_animation(const std::string &name) {
        _active_animation = name;
    }
};
}  // namespace debby

#endif  // DEBBY_COMPONENTS_ANIMATION_COMPONENT_HPP_
