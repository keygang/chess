#pragma once

#include "engine/base/singleton.h"
#include "engine/core/entity.h"
#include "engine/core/manager.h"
#include "engine/core/module.h"
#include "engine/window/window.h"

#include <vector>

namespace engine {

class Engine : public base::Singleton<Engine> {
public:
    struct Params {
        window::Window::Params window_params;
    };

    Engine() = default;

    const Params& get_params() const;
    void set_params(Params params);

    int run(std::unique_ptr<core::Module> module);

    const window::Window* get_window() const;

    template <class T>
    T* get_manager() {
        static_assert(std::is_base_of_v<core::Manager, T>);
        auto it = std::find_if(managers_.begin(), managers_.end(),
                               [](auto& manager) { return dynamic_cast<T*>(manager.get()) != nullptr; });
        if (it == managers_.end()) {
            return nullptr;
        }
        return static_cast<T*>(it->get());
    }

    core::Entity* get_world();

private:
    int init();
    void term(core::Module* module);

private:
    mutable Params params_;
    std::unique_ptr<window::Window> window_ = nullptr;

    std::vector<std::unique_ptr<core::Manager>> managers_;
    core::Entity world_entity_;
};

}  // namespace engine
