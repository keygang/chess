#include "assets_utils.h"


namespace chess::ui::utils {

namespace {

std::string get_current_path() {
  return ASSETS_DIR;
}

Image load_image_data(unsigned char* image_data, int image_width, int image_height) {
  // Create a OpenGL texture identifier
//  GLuint image_texture;
//  glGenTextures(1, &image_texture);
//  glBindTexture(GL_TEXTURE_2D, image_texture);
//
//  // Setup filtering parameters for display
//  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
//  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same
//
//  // Upload pixels into texture
//#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
//  glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
//#endif
//  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
//
//  return {image_texture, image_width, image_height};
    return Image(1, 1, 1);
}


std::optional<std::vector<std::vector<Image>>> load_images_from_file(const char* file_name, int rows, int columns) {
//  int image_width = 0;
//  int image_height = 0;
//  unsigned char* image_data = stbi_load(file_name, &image_width, &image_height, nullptr, 4);
//  if (image_data == nullptr) {
//    return {};
//  }
//
//  int cell_width = image_width / columns;
//  int cell_height = image_height / rows;
//  std::vector<std::vector<Image>> images(rows);
//  for (int row = 0; row < rows; ++row) {
//    for (int column = 0; column < columns; ++column) {
//      std::vector<unsigned char> rect;
//      for (int px = 4 * row; px < 4 * (row + cell_width); ++px) {
//        for (int py = 4 * column; py < 4 * (column + cell_height); ++py) {
//          rect.push_back(image_data[px * 4 * image_width + py]);
//        }
//      }
//      Image image = load_image_data(rect.data(), cell_width, cell_height);
//      images[row].push_back(image);
//    }
//  }
//
//  stbi_image_free(image_data);
//  return images;
    return {};
}


std::optional<Image> load_image_from_file(const char* file_name) {

  // Load from file
//  int image_width = 0;
//  int image_height = 0;
//  unsigned char* image_data = stbi_load(file_name, &image_width, &image_height, nullptr, 4);
//  if (image_data == nullptr) {
//    return {};
//  }
//
//  Image image = load_image_data(image_data, image_width, image_height);
//  stbi_image_free(image_data);
//  return image;
    return Image(1, 1, 1);
}

}

std::optional<Image> load_image_asset(const std::string& asset_name) {
  return load_image_from_file(get_asset_path(asset_name).c_str());
}

std::string get_asset_path(const std::string& asset_name) {
  return get_current_path() + "/" + asset_name;
}

std::optional<std::vector<std::vector<Image>>> load_images_asset(const std::string& asset_name, int rows, int columns) {
  return load_images_from_file(get_asset_path(asset_name).c_str(), rows, columns);
}

}