#pragma once

#include "engine/core/module.h"

namespace chess::ui {

class ChessUIModule : public engine::core::Module {
public:
    void init() override;
    void term() override;
};

}