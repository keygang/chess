#pragma once

#include <list>

namespace engine::base {

template <typename T>
class Singleton {
public:
    static T* get_instance() {
        static auto instance = T();
        return &instance;
    }
};

}  // namespace engine::base
