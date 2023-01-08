#include "chess/core/game.h"

namespace chess::core {

bool ChessGame::move(core::Move move) {
  if (!is_valid_move(move)) {
    return false;
  }
  ChessboardCell& cell = chessboard_[move.from_x][move.from_y];
  chessboard_[move.to_x][move.to_y] = cell;
  cell = std::nullopt;
  moves_.push_back(move);

  return true;
}

const Chessboard& ChessGame::get_chessboard() const { return chessboard_; }

ChessGame::ChessGame() : chessboard_(kDefaultChessboard) {}

bool ChessGame::is_white_move() const {
  return moves_.size() % 2 == 0;
}

bool ChessGame::is_valid_move(const Move& move) {
  const ChessboardCell& chessboard_cell_from = chessboard_[move.from_x][move.from_y];
  if (!chessboard_cell_from) {
    return false;
  }
  const Figure& figure_from = *chessboard_cell_from;
  const ChessboardCell& chessboard_cell_to = chessboard_[move.to_x][move.to_y];

  if (is_white_move()) {
    if (!figure_from.is_white) {
      return false;
    }
    if (chessboard_cell_to && chessboard_cell_to->is_white) {
      return false;
    }
  } else {
    if (figure_from.is_white) {
      return false;
    }
    if (chessboard_cell_to && !chessboard_cell_to->is_white) {
      return false;
    }
  }

//  if (figure_from.figure_name == FigureName::PAWN) {
//    if (figure_from.is_white) {
//      if (move.from_y == chessboard_.size() - 2) {
//        if (move.)
//      }
//    }
//  }

  // TODO: add rules;
  return true;
}

}