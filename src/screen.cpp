#include "screen.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>

#include <glm/ext/vector_int2.hpp>

#include "./globals.h"
#include "./log.h"
#include "./mem.h"

struct BackBuffer {
    debby::Color *buffer;

    BackBuffer();
    ~BackBuffer();

    void initialize() noexcept;

    void set_pixel(const int x, const int y,
                   const debby::Color color) const noexcept;

    void clear() const noexcept;

    void destroy() noexcept;
};

static uint32_t sdl_subsystems{SDL_INIT_VIDEO | SDL_INIT_TIMER |
                               SDL_INIT_EVENTS};
static SDL_DisplayMode display_mode{SDL_DisplayMode()};
static BackBuffer back_buffer{BackBuffer()};
static glm::ivec2 *resolution{nullptr};
static SDL_Texture *front_buffer{nullptr};
static SDL_Window *window{nullptr};
static SDL_Renderer *renderer{nullptr};

static bool initialize_sdl() {
    if (SDL_WasInit(sdl_subsystems)) {
        debby::log::error("SDL has already been initialized");
        return true;
    }
    if (SDL_Init(sdl_subsystems) != 0) {
        debby::log::error("failed to initialize SDL %s", SDL_GetError());
        return false;
    }
    if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) == 0) {
        debby::log::error("failed to initialize SDL Image %s", SDL_GetError());
        return false;
    }
    return true;
}

BackBuffer::BackBuffer() : buffer(nullptr) {}
BackBuffer::~BackBuffer() { destroy(); }

void BackBuffer::initialize() noexcept {
    assert(!buffer && "back buffer already initialized");
    assert(resolution);
    buffer = debby::mem::allocate<debby::Color>(resolution->x * resolution->y);
    debby::log::verbose("initialized backbuffer with resolution (%d,%d)",
                        resolution->x, resolution->y);
    clear();
}

void BackBuffer::set_pixel(const int x, const int y,
                           const debby::Color color) const noexcept {
    assert(buffer && resolution);
    if (x >= 0 && x < resolution->x && y >= 0 && y < resolution->y) {
        buffer[(resolution->x * y) + x] = color;
    }
}

void BackBuffer::clear() const noexcept {
    assert(buffer && resolution);
    for (int i = 0; i < resolution->x * resolution->y; i++) {
        buffer[i] = debby::color::black;
    }
}

void BackBuffer::destroy() noexcept {
    if (buffer) {
        debby::mem::destroy(buffer);
        debby::log::verbose("backbuffer ptr freed");
        buffer = nullptr;
    }
}

bool debby::screen::initialize(Context &ctx) noexcept {
    if (!initialize_sdl()) {
        return false;
    }
    SDL_GetCurrentDisplayMode(0, &display_mode);
    ctx.display_size = glm::ivec2(display_mode.w, display_mode.h);
    if (ctx.resolution.x == 0 || ctx.resolution.y == 0) {
        ctx.resolution = ctx.display_size;
    }
    resolution = &ctx.resolution;
    if (!window) {
        window = SDL_CreateWindow("Debby", SDL_WINDOWPOS_CENTERED,
                                  SDL_WINDOWPOS_CENTERED, ctx.display_size.x,
                                  ctx.display_size.y, SDL_WINDOW_MAXIMIZED);
        if (!window) {
            log::error("failed to create SDL window %s", SDL_GetError());
            return false;
        }
        log::verbose("initialized SDL window (%d,%d)", ctx.display_size.x,
                     ctx.display_size.y);
    }
    if (!renderer) {
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (!renderer) {
            log::error("failed to create SDL renderer %s", SDL_GetError());
            SDL_DestroyWindow(window);
            window = nullptr;
            return false;
        }
        log::verbose("initialized SDL renderer");
    }
    if (!front_buffer) {
        front_buffer = SDL_CreateTexture(renderer, constants::PIXEL_FORMAT,
                                         SDL_TEXTUREACCESS_STREAMING,
                                         resolution->x, resolution->y);
        if (!front_buffer) {
            log::error("failed to create frontbuffer %s", SDL_GetError());
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            renderer = nullptr;
            window = nullptr;
            return false;
        }
        log::verbose("initialized frontbuffer");
    }
    back_buffer.initialize();
    return true;
}

SDL_Renderer *get_renderer() noexcept { return renderer; }

void debby::screen::draw_vec2(const glm::vec2 &vec,
                              const Color color) noexcept {
    back_buffer.set_pixel(vec.x, vec.y, color);
}

void debby::screen::draw_line(const glm::vec2 &p0, const glm::vec2 &p1,
                              const Color color) noexcept {
    auto p0x{static_cast<int>(p0.x)};
    auto p0y{static_cast<int>(p0.y)};
    auto p1x{static_cast<int>(p1.x)};
    auto p1y{static_cast<int>(p1.y)};
    // delta x
    int dx{abs(p1x - p0x)};
    // right or left?
    int sx{p0x < p1x ? 1 : -1};
    int dy{abs(p1y - p0y) * -1};
    // up or down?
    int sy{p0y < p1y ? 1 : -1};
    int error{dx + dy};
    while (true) {
        back_buffer.set_pixel(p0x, p0y, color);
        if (p0x == p1x && p0y == p1y) {
            break;
        }
        int double_err{error * 2};
        if (double_err >= dy) {
            if (p0x == p1x) {
                break;
            }
            error = error + dy;
            p0x += sx;
        }
        if (double_err <= dx) {
            if (p0y == p1y) {
                break;
            }
            error = error + dx;
            p0y += sy;
        }
    }
}

void debby::screen::draw_outlined_rect(const glm::vec2 &center,
                                       const glm::vec2 &size,
                                       const Color color) noexcept {
    auto half_width{static_cast<int>(size.x / 2)};
    auto half_height{static_cast<int>(size.y / 2)};

    auto top_left{glm::vec2{center.x + half_width, center.y - half_height}};
    auto top_right{glm::vec2{center.x - half_width, center.y - half_height}};
    auto bottom_right{glm::vec2{center.x - half_width, center.y + half_height}};
    auto bottom_left{glm::vec2{center.x + half_width, center.y + half_height}};

    draw_line(top_left, top_right, color);
    draw_line(top_right, bottom_right, color);
    draw_line(bottom_right, bottom_left, color);
    draw_line(bottom_left, top_left, color);
}

void debby::screen::draw_filled_rect(const glm::vec2 &center,
                                     const glm::vec2 &size,
                                     const Color color) noexcept {
    for (int y = 0; y < size.x; y++) {
        for (int x = 0; x < size.y; x++) {
            auto x1{static_cast<int>(center.x + x)};
            auto y1{static_cast<int>(center.y + y)};
            back_buffer.set_pixel(x1, y1, color);
        }
    }
}

void debby::screen::clear_renderer() noexcept {
    assert(renderer);
    SDL_SetRenderDrawColor(renderer, color::s_black.r, color::s_black.g,
                           color::s_black.b, color::s_black.a);
    SDL_RenderClear(renderer);
}

void debby::screen::swap_buffers() noexcept {
    assert(front_buffer && back_buffer.buffer && renderer);
    SDL_UpdateTexture(front_buffer, NULL, back_buffer.buffer,
                      resolution->x * sizeof(uint32_t));
    SDL_RenderCopyEx(renderer, front_buffer, NULL, NULL, 0, NULL,
                     SDL_FLIP_NONE);
}

void debby::screen::render_present() noexcept {
    assert(renderer);
    SDL_RenderPresent(renderer);
}

void debby::screen::destroy() noexcept {
    back_buffer.destroy();
    if (front_buffer) {
        SDL_DestroyTexture(front_buffer);
        front_buffer = nullptr;
        log::verbose("destroyed frontbuffer");
    }
    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
        log::verbose("destroyed SDL renderer");
    }
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
        log::verbose("destroyed SDL window");
    }
    log::verbose("quiting all SDL systems");
    SDL_QuitSubSystem(sdl_subsystems);
    SDL_Quit();
}

