#include "image_utils.h"

namespace engine::window {

Image* ImageUtils::load_image(const std::string& path) {
    auto* engine = engine::Engine::get_instance();
    auto* image_manager = engine->get_manager<ImageManager>();
    return image_manager->load_image(path);
}

void ImageUtils::render_image(const IImage* image, int px, int py, float scale) {
    auto* engine = engine::Engine::get_instance();
    auto* renderer = engine->get_manager<Renderer>();
    renderer->render(image, px, py, scale);
}

void ImageUtils::render_image(const IImage* image, int px, int py, int w, int h) {
    auto* engine = engine::Engine::get_instance();
    auto* renderer = engine->get_manager<Renderer>();
    renderer->render(image, px, py, w, h);
}

}