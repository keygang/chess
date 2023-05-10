#pragma once

namespace engine::core {

class System {
public:
    virtual ~System() = default;

    virtual void update() = 0;
};

}  // namespace engine::core