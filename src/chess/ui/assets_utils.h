#pragma once

#include "image.h"

#include <optional>
#include <string>
#include <vector>

namespace chess::ui::utils {

std::optional<Image> load_image_asset(const std::string& asset_name);
std::optional<std::vector<std::vector<Image>>> load_images_asset(const std::string& asset_name, int rows, int columns);

std::string get_asset_path(const std::string& asset_name);

}  // namespace chess::ui::utils