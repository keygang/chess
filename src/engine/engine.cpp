#include "engine/engine.h"

namespace engine {

Engine::Engine(Params params) : params_(std::move(params)) {
    window_ = std::make_unique<window::Window>(params_.window_params);
}

int Engine::init(core::Module* module) {
    if (window_) {
        if (int code = window_->init()) {
            return code;
        }
    }

    module->init();

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

    init(module.get());

    while (window_->new_frame()) {
        module->update();

        window_->render();
    }

    term(module.get());

    return 0;
}

}  // namespace engine