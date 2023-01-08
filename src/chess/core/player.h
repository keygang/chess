#pragma once

#include "chess/core/chessboard.h"
#include "chess/core/move.h"

namespace chess::core {

class PlayerI {
public:
  virtual void move(Move move) = 0;
};

}