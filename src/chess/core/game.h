#pragma once

#include "chess/core/player.h"

#include <vector>

namespace chess::core {

class ChessGame {
public:
  ChessGame();
  void reset();

  const Chessboard& get_chessboard() const;
  bool move(Move move);
  void pop_move(); // TODO: optimize

  bool is_valid_move(const Move& move) const;
  std::vector<Move> get_valid_moves(const CellIndex& from_index) const;
  bool is_white_move() const;

private:
  // Only for valid moves
  void push_move(const Move& move);
  bool is_pawn_en_passant(const Move& move) const;
  bool is_castling(const Move& move) const;
  bool is_check_move(const Move& move);
  bool is_moved(const CellIndex& cell_index) const;

  Chessboard chessboard_;

  std::vector<Move> moves_;
  std::vector<Move> current_valid_moves_;
};

}  // namespace chess::core