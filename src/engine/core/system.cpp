#include "engine/core/system.h"
#include "engine/engine.h"

namespace engine::core {

System::System() {
    auto* engine = engine::Engine::get_instance();
    world_ = engine->get_world();
}

core::Entity* System::get_world() {
    return world_;
}

const core::Entity* System::get_world() const {
    return world_;
}

}  // namespace engine::core
