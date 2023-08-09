#pragma once

#include "engine/core/entity.h"

namespace engine::core {

class System {
public:
    System();

    virtual ~System() = default;

    virtual void update() = 0;

    core::Entity* get_world();

    const core::Entity* get_world() const;

protected:
    core::Entity* world_;
};

}  // namespace engine::core