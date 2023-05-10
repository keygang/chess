#pragma once

#include "engine/core/system.h"

#include <vector>

namespace engine::core {

class Engine;

class Module {
public:
    virtual ~Module() = default;

    virtual void init() = 0;
    virtual void term() = 0;

    void update();

    void move(Module* to);

protected:
    void register_system(std::unique_ptr<core::System> system);

private:
    std::vector<std::unique_ptr<core::System>> systems_;
};

}