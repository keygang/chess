#pragma once

#include "engine/base/types.h"
#include "engine/core/manager.h"

#include <SDL_image.h>

#include <string>
#include <vector>

namespace engine::window {

class IImage {
public:
    virtual int width() const = 0;
    virtual int height() const = 0;

    virtual base::Rect<int> rect() const = 0;

    virtual SDL_Texture* texture() const = 0;
};

class Image : public IImage {
public:
    ~Image();
    Image(SDL_Texture* texture, int width, int height);

    int width() const override;
    int height() const override;
    base::Rect<int> rect() const override;

    SDL_Texture* texture() const override;

private:
    SDL_Texture* texture_;
    int width_ = 0;
    int height_ = 0;
};

class Sprite : public IImage {
public:
    Sprite() = default;
    Sprite(Image* image, base::Rect<int> rect);

    int width() const override;
    int height() const override;
    base::Rect<int> rect() const override;
    SDL_Texture* texture() const override;

private:
    Image* image_ = nullptr;
    base::Rect<int> rect_;
};

class Sprites {
public:
    Sprites(Image* image, int rows, int columns);

    Sprite* index(int i);

    const Sprite* index(int i) const;

private:
    std::vector<std::vector<Sprite>> sprites_;
};

class ImageManager : public core::Manager {
public:
    explicit ImageManager(SDL_Renderer* renderer);

    Image* load_image(const std::string& path);

private:
    SDL_Renderer* renderer_ = nullptr;
    std::vector<std::unique_ptr<Image>> images_;
};

}  // namespace engine::window