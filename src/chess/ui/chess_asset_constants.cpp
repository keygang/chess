#include "chess/ui/chess_asset_constants.h"

namespace chess::ui {

ChessAssetConstants figure_to_asset_constant(const chess::core::Figure& figure) {
  static const std::unordered_map<chess::core::Figure, ChessAssetConstants> mapping = {
      {{chess::core::FigureName::BISHOP, true}, ChessAssetConstants::BISHOP_WHITE},
      {{chess::core::FigureName::KNIGHT, true}, ChessAssetConstants::KNIGHT_WHITE},
      {{chess::core::FigureName::PAWN, true}, ChessAssetConstants::PAWN_WHITE},
      {{chess::core::FigureName::QUEEN, true}, ChessAssetConstants::QUEEN_WHITE},
      {{chess::core::FigureName::KING, true}, ChessAssetConstants::KING_WHITE},
      {{chess::core::FigureName::ROOK, true}, ChessAssetConstants::ROOK_WHITE},

      {{chess::core::FigureName::BISHOP, false}, ChessAssetConstants::BISHOP_BLACK},
      {{chess::core::FigureName::KNIGHT, false}, ChessAssetConstants::KNIGHT_BLACK},
      {{chess::core::FigureName::PAWN, false}, ChessAssetConstants::PAWN_BLACK},
      {{chess::core::FigureName::QUEEN, false}, ChessAssetConstants::QUEEN_BLACK},
      {{chess::core::FigureName::KING, false}, ChessAssetConstants::KING_BLACK},
      {{chess::core::FigureName::ROOK, false}, ChessAssetConstants::ROOK_BLACK},
  };

  return mapping.at(figure);
}

}
