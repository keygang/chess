#pragma once

#include <chess/core/figure.h>

#include <unordered_map>

namespace chess::ui {

enum class ChessAssetConstants {
  KING_WHITE,
  QUEEN_WHITE,
  ROOK_WHITE,
  BISHOP_WHITE,
  KNIGHT_WHITE,
  PAWN_WHITE,
  KING_BLACK,
  QUEEN_BLACK,
  ROOK_BLACK,
  BISHOP_BLACK,
  KNIGHT_BLACK,
  PAWN_BLACK,
  BOARD,

  NONE,
};

ChessAssetConstants figure_to_asset_constant(const chess::core::Figure& figure);

}
