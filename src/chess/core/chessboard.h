#pragma once

#include "chess/core/figure.h"

#include <array>
#include <optional>
#include <vector>

namespace chess::core {

static const int kRowsNum = 8;
static const int kColumnsNum = 8;

struct CellIndex {
    int x = -1;
    int y = -1;

    bool operator==(const CellIndex& rhs) const;
    bool operator!=(const CellIndex& rhs) const;
    bool operator<(const CellIndex& rhs) const;
};

struct Move {
    CellIndex from;
    CellIndex to;

    bool operator==(const Move& rhs) const;
    bool operator!=(const Move& rhs) const;
    bool operator<(const Move& rhs) const;
};

class Chessboard {
public:
    Chessboard(std::initializer_list<std::array<std::optional<Figure>, kRowsNum>> init_list);
    explicit Chessboard(const std::array<std::array<std::optional<Figure>, kRowsNum>, kColumnsNum>& chessboard);

    size_t size() const;

    std::array<std::optional<Figure>, kRowsNum>& operator[](size_t index);
    const std::array<std::optional<Figure>, kRowsNum>& operator[](size_t index) const;

    std::optional<Figure>& operator[](const CellIndex& index);
    const std::optional<Figure>& operator[](const CellIndex& index) const;

    bool is_in_range(const Move& move) const;
    bool is_in_range(const CellIndex& cell_index) const;
    std::optional<CellIndex> find_figure(const Figure& figure) const;
    std::vector<Move> get_attack_moves(const CellIndex& from_index) const;
    std::vector<Move> get_all_attack_moves(bool is_white) const;

private:
    std::array<std::array<std::optional<Figure>, kRowsNum>, kColumnsNum> chessboard_;
};

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