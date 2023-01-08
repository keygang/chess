#pragma once

#include "chess/core/player.h"

#include <vector>

namespace chess::core {

class ChessGame {
public:
  ChessGame();

  const Chessboard& get_chessboard() const;
  bool move(Move move);

  bool is_valid_move(const Move& move);
  bool is_white_move() const;

private:
  Chessboard chessboard_;

  std::vector<Move> moves_;
};

}  // namespace chess::core