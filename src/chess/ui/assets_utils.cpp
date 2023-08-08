#include "assets_utils.h"

#include <SDL_opengl.h>

namespace chess::ui::utils {

std::string get_current_path() {
    return ASSETS_DIR;
}

std::string get_asset_path(const std::string& asset_name) {
    return get_current_path() + "/" + asset_name;
}

}  // namespace chess::ui::utils