#include "asset_manager.hpp"

#include <map>

#include "SDL2/SDL_image.h"

static std::map<std::string, SDL_Texture*> textures{};

void debby::managers::asset::add_texture(const std::string& texture_id,
                                         const std::string& file_path) {
    SDL_Surface* surface{IMG_Load(file_path.c_str())};
}

SDL_Texture* debby::managers::asset::get_texture(
    const std::string& texture_id) {
    return nullptr;
}

void debby::managers::asset::clear_assets() {}

