#pragma once

namespace chess::ui {

class Image {
public:
    Image(int id, int width, int height);
    int get_id() const;
    int get_width() const;
    int get_height() const;

private:
    int id_;
    int width_;
    int height_;
};

class ImageInfo {
public:
};

}  // namespace chess::ui