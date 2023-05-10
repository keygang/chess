#include "engine/core/module.h"

namespace engine::core {

void Module::move(Module* to) {
    to->systems_.insert(to->systems_.end(), std::make_move_iterator(systems_.begin()),
                        std::make_move_iterator(systems_.end()));
}

void Module::update() {
    for (std::unique_ptr<System>& system : systems_) {
        system->update();
    }
}

void Module::register_system(std::unique_ptr<core::System> system) {
    systems_.push_back(std::move(system));
}

}  // namespace engine::core