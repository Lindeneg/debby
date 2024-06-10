#ifndef DEBBY_COMMON_GLOBALS_H_
#define DEBBY_COMMON_GLOBALS_H_

#include <SDL2/SDL_pixels.h>

namespace debby {

using Color = SDL_Color;
}  // namespace debby

namespace debby::constants {
// target FPS for fixed updates
constexpr int FPS{60};

// target time in ms for each frame
constexpr int FRAME_TARGET{1000 / FPS};

// maximum delta time (useful if running in debugger)
constexpr double MAXIMUM_DT{0.05};

/* used to produce epsilon relative
 * to value of some given operands */
constexpr double REL_EPSILON{1e-8};

// absolute epsilon value
constexpr double ABS_EPSILON{1e-12};

// approximation of PI
constexpr double PI{3.14159265359};

// twice the pi, double the fun!
constexpr double PI2{PI * 2};

}  // namespace debby::constants

namespace debby::color {
constexpr Color white{255, 255, 255, 255};
constexpr Color black{0, 0, 0, 255};
constexpr Color red{255, 0, 0, 255};
constexpr Color green{0, 255, 0, 255};
constexpr Color blue{0, 0, 255, 255};
constexpr Color yellow{255, 255, 0, 255};
constexpr Color cyan{0, 255, 255, 255};
constexpr Color magenta{255, 0, 255, 255};
constexpr Color gray{128, 128, 128, 255};
constexpr Color light_gray{211, 211, 211, 255};
constexpr Color dark_gray{64, 64, 64, 255};
constexpr Color orange{255, 165, 0, 255};
constexpr Color pink{255, 192, 203, 255};
constexpr Color purple{128, 0, 128, 255};
constexpr Color brown{165, 42, 42, 255};
constexpr Color light_blue{173, 216, 230, 255};
constexpr Color light_green{144, 238, 144, 255};
constexpr Color dark_red{139, 0, 0, 255};
constexpr Color dark_green{0, 100, 0, 255};
constexpr Color dark_blue{0, 0, 139, 255};
}  // namespace debby::color

#endif  // DEBBY_COMMON_GLOBALS_H_
