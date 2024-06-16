#ifndef DEBBY_COMPONENTS_SPRITE_COMPONENT_HPP_
#define DEBBY_COMPONENTS_SPRITE_COMPONENT_HPP_

#include <SDL2/SDL_rect.h>

#include <string>
#include <utility>

namespace debby {

/* SpriteComponent allows an entity to have a
 * relation to an asset, such that it can be rendered */
class SpriteComponent {
   public:
    std::string asset_id;
    int width;
    int height;
    // TODO make a layer system instead
    int z_index;
    int src_x;
    int src_y;

    explicit SpriteComponent(std::string asset_id = "", int width = 0,
                             int height = 0, int z_index = 0, int src_x = 0,
                             int src_y = 0)
        : asset_id(std::move(asset_id)),
          width(width),
          height(height),
          z_index(z_index),
          src_x(src_x),
          src_y(src_y) {}

    ~SpriteComponent() = default;

    [[nodiscard]] inline SDL_Rect get_rect() const {
        return {src_x, src_y, width, height};
    }
};

}  // namespace debby

#endif  // DEBBY_COMPONENTS_SPRITE_COMPONENT_HPP_
