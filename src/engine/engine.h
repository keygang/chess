#pragma once

#include "engine/core/module.h"
#include "engine/window/window.h"

#include <vector>

namespace engine {

class Engine {
public:
    struct Params {
        window::Window::Params window_params;
    };

    explicit Engine(Params params);

    int run(std::unique_ptr<core::Module> module);

private:
    int init(core::Module* module);
    void term(core::Module* module);

private:
    Params params_;
    std::unique_ptr<window::Window> window_ = nullptr;
};

}  // namespace engine