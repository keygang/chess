#pragma once

#include "chess/core/chessboard.h"

namespace chess::core {

class PlayerI {
public:
    virtual ~PlayerI() = default;
    virtual void move(Move move) = 0;
};

}  // namespace chess::core