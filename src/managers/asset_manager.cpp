#include "asset_manager.hpp"

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include <spdlog/spdlog.h>

#include <map>

#include "./screen_manager.hpp"

static std::map<std::string, SDL_Texture *> textures{};

bool debby::managers::asset::add_texture(const std::string &texture_id,
                                         const std::string &file_path) {
    SDL_Texture *texture{
        IMG_LoadTexture(screen::get_renderer(), file_path.c_str())};
    if (!texture) {
        spdlog::error("failed to load texture '{0}' with path '{1}'",
                      texture_id, file_path);
        return false;
    }
    spdlog::debug("adding texture '{0}' with path '{1}'", texture_id,
                  file_path);
    textures.emplace(texture_id, texture);
    return true;
}

SDL_Texture *debby::managers::asset::get_texture(
    const std::string &texture_id) {
    auto iter{textures.find(texture_id)};
    if (iter == textures.end()) {
        spdlog::warn("texture '{0}' does not exist", texture_id);
        return nullptr;
    }
    return iter->second;
}

void debby::managers::asset::destroy() {
    for (const auto &texture : textures) {
        spdlog::debug("removing texture '{0}'", texture.first);
        SDL_DestroyTexture(texture.second);
    }
    textures.clear();
}

