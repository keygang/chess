#pragma once

#include "chess/core/player.h"

#include <vector>

namespace chess::core {

class ChessGame {
public:
  static const std::vector<FigureName> kValidPromotions;

  ChessGame();
  void reset();

  const Chessboard& get_chessboard() const;
  bool move(const std::variant<Move, FigureName>& move);
  bool is_promotion() const;

  void pop_move(); // TODO: optimize

  bool is_valid_move(const std::variant<Move, FigureName>& move) const;
  std::vector<Move> get_valid_moves(const CellIndex& from_index) const;
  bool is_white_move() const;

private:
  bool is_valid_promotion(FigureName figure_name) const;
  bool is_valid_simple_move(const Move& move) const;

  // Only for valid moves
  void push_move(const std::variant<Move, FigureName>& move);
  bool is_pawn_en_passant(const Move& move) const;
  bool is_castling(const Move& move) const;
  bool is_check_move(const Move& move);
  bool is_moved(const CellIndex& cell_index) const;

  Chessboard chessboard_;

  std::vector<std::variant<Move, FigureName>> moves_;
  size_t moves_number_ = 0;

  bool is_promotion_ = false;
};

}  // namespace chess::core