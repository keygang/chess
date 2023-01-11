#pragma once

#include "chess/core/chessboard.h"

namespace chess::core {

class PlayerI {
public:
  virtual void move(Move move) = 0;
};

}