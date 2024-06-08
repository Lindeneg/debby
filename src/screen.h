#ifndef DEBBY_SCREEN_H_
#define DEBBY_SCREEN_H_

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>

#include <glm/vec2.hpp>

#include "./globals.h"

namespace debby::screen {

bool initialize(Context &ctx) noexcept;

void draw_vec2(const glm::vec2 &vec, const Color color) noexcept;

void draw_line(const glm::vec2 &p0, const glm::vec2 &p1,
               const Color color) noexcept;

void draw_outlined_rect(const glm::vec2 &center, const glm::vec2 &size,
                        const Color color) noexcept;

void draw_filled_rect(const glm::vec2 &center, const glm::vec2 &size,
                      const Color color) noexcept;

SDL_Renderer *get_renderer() noexcept;

void clear_renderer() noexcept;

void swap_buffers() noexcept;

void render_present() noexcept;

void destroy() noexcept;
}  // namespace debby::screen

#endif  // DEBBY_SCREEN_H_
