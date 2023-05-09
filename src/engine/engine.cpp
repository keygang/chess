#include "engine/engine.h"

namespace engine {

void Engine::run(std::unique_ptr<core::Module> start_module) {
    while (true) {
        for (const std::unique_ptr<core::System>& system : start_module->systems_) {
            system->update();
        }
    }
}

}