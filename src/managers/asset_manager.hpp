#ifndef DEBBY_MANAGERS_ASSET_MANAGER_H_
#define DEBBY_MANAGERS_ASSET_MANAGER_H_

#include <SDL2/SDL_render.h>

#include <map>
#include <string>

namespace debby::manager {

class AssetManager {
   private:
    std::map<std::string, SDL_Texture*> _textures;

   public:
    AssetManager();
    ~AssetManager();

    void add_texture(const std::string& texture_id,
                     const std::string& file_path);

    SDL_Texture* get_texture(const std::string& texture_id) const;
};
}  // namespace debby::manager

#endif  // DEBBY_MANAGERS_ASSET_MANAGER_H_
