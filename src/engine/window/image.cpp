#include "engine/window/image.h"

namespace engine::window {

Image::Image(SDL_Texture* texture, int width, int height) : texture_(texture), width_(width), height_(height) {}

Image::~Image() {
    SDL_DestroyTexture(texture_);
}

int Image::width() const {
    return width_;
}

int Image::height() const {
    return height_;
}

//void Image::set_height(int height) {
//    SDL_QueryTexture(texture_, nullptr, nullptr, nullptr, &height_);
//    height_ = height;
//}
//
//void Image::set_width(int width) {
//    SDL_QueryTexture(texture_, nullptr, nullptr, &width_, nullptr);
//    width_ = width;
//}

SDL_Texture* Image::texture() const {
    return texture_;
}

base::Rect<int> Image::rect() const {
    return {0, 0, width(), height()};
}

Image* ImageManager::load_image(const std::string& path) {
    SDL_Surface* imageSurface = IMG_Load(path.c_str());
    if (!imageSurface) {
        return nullptr;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer_, imageSurface);
    //    SDL_RenderCopy(renderer_, texture, nullptr, nullptr);
    images_.emplace_back(std::make_unique<Image>(texture, imageSurface->w, imageSurface->h));
    return images_.back().get();
}

ImageManager::ImageManager(SDL_Renderer* renderer) : renderer_(renderer) {}

Sprite::Sprite(Image* image, base::Rect<int> rect) : image_(image), rect_(rect) {}

int Sprite::width() const {
    return rect_[1] - rect_[0];
}

int Sprite::height() const {
    return rect_[2] - rect_[0];
}

base::Rect<int> Sprite::rect() const {
    return rect_;
}

SDL_Texture* Sprite::texture() const {
    return image_->texture();
}

Sprites::Sprites(Image* image, int rows, int columns) {
    assert(image->width() % columns == 0);
    assert(image->height() % rows == 0);
    int sprite_width = image->width() / columns;
    int sprite_height = image->height() / rows;
    sprites_.resize(rows);
    for (int i = 0; i < rows; ++i) {
        sprites_[i].resize(columns);
        for (int j = 0; j < columns; ++j) {
            sprites_[i][j] =
                Sprite(image, {i * sprite_width, j * sprite_height, (i + 1) * sprite_width, (j + 1) * sprite_height});
        }
    }
}

Sprite* Sprites::index(int i) {
    return &sprites_[i / sprites_.size()][i % sprites_.size()];
}

const Sprite* Sprites::index(int i) const {
    return &sprites_[i / sprites_.size()][i % sprites_.size()];
}

}  // namespace engine::window
