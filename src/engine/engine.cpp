#include "engine/engine.h"
#include "engine/window/image.h"
#include "engine/window/render/renderer.h"

namespace engine {

int Engine::init() {
    window_ = std::make_unique<window::Window>(params_.window_params);

    if (window_) {
        if (int code = window_->init()) {
            return code;
        }

        assert(window_->get_renderer());
        managers_.emplace_back(std::make_unique<window::ImageManager>(window_->get_renderer()));
        managers_.emplace_back(std::make_unique<window::Renderer>(window_->get_renderer()));
    }

    return 0;
}

void Engine::term(core::Module* module) {
    module->term();

    if (window_) {
        window_->term();
    }
}

int Engine::run(std::unique_ptr<core::Module> module) {
    assert(module);

    init();
    module->init();

    while (window_->new_frame()) {
        module->update();

        window_->render();
    }

    term(module.get());

    return 0;
}

void Engine::set_params(Engine::Params params) {
    params_ = params;
    if (window_) {
        window_->set_params(params_.window_params);
    }
}

const window::Window* Engine::get_window() const {
    return window_.get();
}

const Engine::Params& Engine::get_params() const {
    return params_;
}

core::Entity* Engine::get_world() {
    return &world_entity_;
}

}  // namespace engine