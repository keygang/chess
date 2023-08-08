#include "engine/window/render/renderer.h"

namespace engine::window {

void Renderer::render(const IImage* image, int px, int py, float scale) {
    render(image, px, py, image->width() * scale, image->height() * scale);
}

void Renderer::render(const IImage* image, int px, int py, int w, int h) {
    SDL_Rect dst_rect = {px, py,
                         w, h};
    base::Rect<int> rect = image->rect();
    SDL_Rect src_rect = {rect[0], rect[1], rect[2], rect[3]};
    SDL_RenderCopy(renderer_, image->texture(), &src_rect, &dst_rect);
}

Renderer::Renderer(SDL_Renderer* renderer) : renderer_(renderer) {}

}