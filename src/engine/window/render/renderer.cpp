#include "engine/window/render/renderer.h"
#include <iostream>

namespace engine::window {

void Renderer::render(const IImage* image, int px, int py, float scale) {
    render(image, px, py, image->width() * scale, image->height() * scale);
}

void Renderer::render(const IImage* image, int px, int py, int w, int h) {
    SDL_Rect dst_rect = {px, py, w, h};
    base::Rect<int> rect = image->rect();
    SDL_Rect src_rect = {rect[0], rect[1], rect[2], rect[3]};
    SDL_RenderCopy(renderer_, image->texture(), &src_rect, &dst_rect);
}

Renderer::Renderer(SDL_Renderer* renderer) : renderer_(renderer) {}

void Renderer::render(const base::Rect<int>& rect, const base::Color& color) {
    SDL_SetRenderDrawBlendMode(renderer_, SDL_BLENDMODE_BLEND);
    SDL_Rect sdl_rect = {rect[0], rect[1], rect[2], rect[3]};
    if (SDL_SetRenderDrawColor(renderer_, Uint8(color[0] * 255), Uint8(color[1] * 255), Uint8(color[2] * 255),
                               Uint8(color[3] * 255))) {
        std::cout << SDL_GetError() << "\n";
    }

    SDL_RenderFillRect(renderer_, &sdl_rect);
}

}  // namespace engine::window
