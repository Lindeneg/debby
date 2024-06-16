#ifndef DEBBY_MANAGERS_ASSET_MANAGER_HPP_
#define DEBBY_MANAGERS_ASSET_MANAGER_HPP_

#include <SDL2/SDL_render.h>

#include <string>

namespace debby::managers::asset {

bool add_texture(const std::string &texture_id, const std::string &file_path);

SDL_Texture *get_texture(const std::string &texture_id);

void destroy();
}  // namespace debby::managers::asset

#endif  // DEBBY_MANAGERS_ASSET_MANAGER_HPP_
