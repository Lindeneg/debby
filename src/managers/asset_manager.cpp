#include "asset_manager.hpp"

#include "SDL2/SDL_image.h"

debby::manager::AssetManager::AssetManager() {}

debby::manager::AssetManager::~AssetManager() {}

void debby::manager::AssetManager::add_texture(const std::string& texture_id,
                                               const std::string& file_path) {
    SDL_Surface* surface{IMG_Load(file_path.c_str())};
}

SDL_Texture* debby::manager::AssetManager::get_texture(
    const std::string& texture_id) const {
    return nullptr;
}

void debby::manager::AssetManager::clear_assets() {}
