#pragma once

#include "engine/core/manager.h"
#include "engine/window/image.h"

namespace engine::window {

class Renderer : public core::Manager {
public:
    explicit Renderer(SDL_Renderer* renderer);

    void render(const IImage* image, int px, int py, float scale = 1.f);
    void render(const IImage* image, int px, int py, int w, int h);

private:
    SDL_Renderer* renderer_;
};

}  // namespace engine::window
