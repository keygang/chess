#pragma once

#include "chess/core/figure.h"

#include <array>
#include <optional>

namespace chess::core {

static const int kRowsNum = 8;
static const int kColumnsNum = 8;

using ChessboardCell = std::optional<Figure>;
using Chessboard = std::array<std::array<ChessboardCell, kRowsNum>, kColumnsNum>;

static const Chessboard kDefaultChessboard = {{{{
                                                   {{FigureName::ROOK, false}},
                                                   {{FigureName::KNIGHT, false}},
                                                   {{FigureName::BISHOP, false}},
                                                   {{FigureName::QUEEN, false}},
                                                   {{FigureName::KING, false}},
                                                   {{FigureName::BISHOP, false}},
                                                   {{FigureName::KNIGHT, false}},
                                                   {{FigureName::ROOK, false}},
                                               }},
                                               {{
                                                   {{FigureName::PAWN, false}},
                                                   {{FigureName::PAWN, false}},
                                                   {{FigureName::PAWN, false}},
                                                   {{FigureName::PAWN, false}},
                                                   {{FigureName::PAWN, false}},
                                                   {{FigureName::PAWN, false}},
                                                   {{FigureName::PAWN, false}},
                                                   {{FigureName::PAWN, false}},
                                               }},
                                               {},
                                               {},
                                               {},
                                               {},
                                               {{
                                                   {{FigureName::PAWN, true}},
                                                   {{FigureName::PAWN, true}},
                                                   {{FigureName::PAWN, true}},
                                                   {{FigureName::PAWN, true}},
                                                   {{FigureName::PAWN, true}},
                                                   {{FigureName::PAWN, true}},
                                                   {{FigureName::PAWN, true}},
                                                   {{FigureName::PAWN, true}},
                                               }},
                                               {{
                                                   {{FigureName::ROOK, true}},
                                                   {{FigureName::KNIGHT, true}},
                                                   {{FigureName::BISHOP, true}},
                                                   {{FigureName::QUEEN, true}},
                                                   {{FigureName::KING, true}},
                                                   {{FigureName::BISHOP, true}},
                                                   {{FigureName::KNIGHT, true}},
                                                   {{FigureName::ROOK, true}},
                                               }}}};

}  // namespace chess::core