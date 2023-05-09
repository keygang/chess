#include "engine/core/module.h"

namespace engine::core {

void Module::move(Module* to) {
    to->systems_.insert(to->systems_.end(),
                        std::make_move_iterator(systems_.begin()),
                        std::make_move_iterator(systems_.end()));
}

}