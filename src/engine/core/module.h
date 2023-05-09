#pragma once

#include "engine/core/system.h"

#include <vector>

namespace engine::core {

class Engine;

class Module {
public:
    virtual ~Module() = 0;

    virtual void init() = 0;
    virtual void term() = 0;

    void move(Module* to);

private:
    std::vector<std::unique_ptr<core::System>> systems_;

    friend Engine;
};

}