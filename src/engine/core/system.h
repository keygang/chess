#pragma once

namespace engine::core {

class System {
public:
    virtual ~System() = 0;

    virtual void update() = 0;
};

}  // namespace engine::core