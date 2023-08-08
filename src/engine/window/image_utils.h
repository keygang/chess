#pragma once

#include "engine/engine.h"
#include "engine/window/render/renderer.h"

namespace engine::window {

class ImageUtils {
public:
    static Image* load_image(const std::string& path);
    static void render_image(const IImage* image, int px, int py, float scale = 1.f);
    static void render_image(const IImage* image, int px, int py, int w, int h);
};

}