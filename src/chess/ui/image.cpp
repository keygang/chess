#include "image.h"

namespace chess::ui {

Image::Image(GLuint id, int width, int height) : id_(id), width_(width), height_(height) {}
GLuint Image::get_id() const {
  return id_;
}
int Image::get_width() const {
  return width_;
}
int Image::get_height() const {
  return height_;
}

}  // namespace chess::ui
