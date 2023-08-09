#pragma once

#include "engine/core/component.h"

#include <vector>

namespace engine::core {

class Entity {
public:
    template <typename TComponent>
    TComponent* add_component() {
        components_.push_back(std::make_unique<TComponent>());
        return static_cast<TComponent*>(components_.back().get());
    }

    template <typename TComponent>
    TComponent* get_component_w() {
        for (const std::unique_ptr<Component>& component : components_) {
            if (auto* c = dynamic_cast<TComponent*>(component.get())) {
                return c;
            }
        }
        return nullptr;
    }

    template <typename TComponent>
    const TComponent* get_component() const {
        return get_component_w<TComponent>();
    }

private:
    std::vector<std::unique_ptr<Component>> components_;
};

}  // namespace engine::core
