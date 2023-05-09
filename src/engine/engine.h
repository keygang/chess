#pragma once

#include "engine/core/module.h"

#include <vector>

namespace engine {

class Engine {
public:
    void run(std::unique_ptr<core::Module> start_module);
    void register_system(std::unique_ptr<core::System> system);

private:

};

}