#pragma once

#include "GLFW/glfw3.h"

namespace chess::ui {

class Image {
public:
  Image(GLuint id, int width, int height);
  GLuint get_id() const;
  int get_width() const;
  int get_height() const;

private:
  GLuint id_;
  int width_;
  int height_;
};

class ImageInfo {
public:

};

}  // namespace chess::ui