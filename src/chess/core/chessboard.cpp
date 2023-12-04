#include "chess/core/chessboard.h"

#include <cassert>

namespace chess::core {

bool CellIndex::operator<(const CellIndex& rhs) const {
    return x < rhs.x || (x == rhs.x && y < rhs.y);
}
bool CellIndex::operator==(const CellIndex& rhs) const {
    return x == rhs.x && y == rhs.y;
}
bool CellIndex::operator!=(const CellIndex& rhs) const {
    return !(rhs == *this);
}

bool Move::operator<(const Move& rhs) const {
    return (from < rhs.from) || (from == rhs.from && to < rhs.to);
}
bool Move::operator==(const Move& rhs) const {
    return from == rhs.from && to == rhs.to;
}
bool Move::operator!=(const Move& rhs) const {
    return !(rhs == *this);
}

std::array<std::optional<Figure>, kRowsNum>& Chessboard::operator[](size_t index) {
    return chessboard_[index];
}
const std::array<std::optional<Figure>, kRowsNum>& Chessboard::operator[](size_t index) const {
    return chessboard_[index];
}

Chessboard::Chessboard(const std::array<std::array<std::optional<Figure>, kRowsNum>, kColumnsNum>& chessboard)
    : chessboard_(chessboard) {}

Chessboard::Chessboard(std::initializer_list<std::array<std::optional<Figure>, kRowsNum>> init_list) {
    assert(init_list.size() == chessboard_.size());

    size_t i = 0;
    for (const auto& it : init_list) {
        chessboard_[i++] = it;
        if (chessboard_.size() == i) {
            break;
        }
    }
}

int Chessboard::size() const {
    return static_cast<int>(chessboard_.size());
}

bool Chessboard::is_in_range(const Move& move) const {
    return is_in_range(move.from) && is_in_range(move.to);
}

bool Chessboard::is_in_range(const CellIndex& cell_index) const {
    return !(cell_index.x < 0 ||
             cell_index.x >= static_cast<int>(chessboard_.size()) ||
             cell_index.y < 0 ||
             cell_index.y >= static_cast<int>(chessboard_.size()));
}

std::optional<Figure>& Chessboard::operator[](const CellIndex& index) {
    return chessboard_[index.x][index.y];
}

const std::optional<Figure>& Chessboard::operator[](const CellIndex& index) const {
    return chessboard_[index.x][index.y];
}

std::optional<CellIndex> Chessboard::find_figure(const Figure& figure) const {
    for (size_t row = 0; row < chessboard_.size(); ++row) {
        for (size_t column = 0; column < chessboard_[row].size(); ++column) {
            if (figure == chessboard_[row][column]) {
                return {{static_cast<int>(row), static_cast<int>(column)}};
            }
        }
    }
    return std::nullopt;
}
std::vector<Move> Chessboard::get_attack_moves(const CellIndex& from_index) const {
    if (!is_in_range(from_index)) {
        return {};
    }
    const std::optional<Figure>& figure = chessboard_[from_index.x][from_index.y];
    if (!figure) {
        return {};
    }

    std::vector<Move> moves;

    // Return `false` if need to stop.
    auto add_if_valid = [this, &moves, &from_index](const CellIndex& to_index) {
        Move move;
        move.from = from_index;
        move.to = to_index;
        if (!is_in_range(move.to)) {
            return false;
        }
        if (chessboard_[move.to.x][move.to.y]) {
            if (chessboard_[move.from.x][move.from.y]->is_white != chessboard_[move.to.x][move.to.y]->is_white) {
                moves.push_back(move);
            }
            return false;
        }

        moves.push_back(move);
        return true;
    };

    auto add_king_moves = [&add_if_valid, &from_index]() {
        add_if_valid({from_index.x - 1, from_index.y});
        add_if_valid({from_index.x - 1, from_index.y + 1});
        add_if_valid({from_index.x - 1, from_index.y - 1});
        add_if_valid({from_index.x + 1, from_index.y});
        add_if_valid({from_index.x + 1, from_index.y + 1});
        add_if_valid({from_index.x + 1, from_index.y - 1});
        add_if_valid({from_index.x, from_index.y - 1});
        add_if_valid({from_index.x, from_index.y + 1});
    };

    auto add_rook_moves = [&add_if_valid, &from_index]() {
        for (int row = 1; row < kRowsNum; ++row) {
            if (!add_if_valid({from_index.x - row, from_index.y})) {
                break;
            }
        }
        for (int row = 1; row < kRowsNum; ++row) {
            if (!add_if_valid({from_index.x + row, from_index.y})) {
                break;
            }
        }
        for (int column = 1; column < kColumnsNum; ++column) {
            if (!add_if_valid({from_index.x, from_index.y - column})) {
                break;
            }
        }
        for (int column = 1; column < kColumnsNum; ++column) {
            if (!add_if_valid({from_index.x, from_index.y + column})) {
                break;
            }
        }
    };

    auto add_knight_moves = [&add_if_valid, &from_index]() {
        add_if_valid({from_index.x - 1, from_index.y + 2});
        add_if_valid({from_index.x - 1, from_index.y - 2});
        add_if_valid({from_index.x + 1, from_index.y + 2});
        add_if_valid({from_index.x + 1, from_index.y - 2});

        add_if_valid({from_index.x - 2, from_index.y + 1});
        add_if_valid({from_index.x - 2, from_index.y - 1});
        add_if_valid({from_index.x + 2, from_index.y + 1});
        add_if_valid({from_index.x + 2, from_index.y - 1});
    };

    auto add_bishop_moves = [&add_if_valid, &from_index]() {
        for (int row = 1; row < kRowsNum; ++row) {
            if (!add_if_valid({from_index.x - row, from_index.y - row})) {
                break;
            }
        }
        for (int row = 1; row < kRowsNum; ++row) {
            if (!add_if_valid({from_index.x + row, from_index.y - row})) {
                break;
            }
        }
        for (int row = 1; row < kRowsNum; ++row) {
            if (!add_if_valid({from_index.x - row, from_index.y + row})) {
                break;
            }
        }
        for (int row = 1; row < kRowsNum; ++row) {
            if (!add_if_valid({from_index.x + row, from_index.y + row})) {
                break;
            }
        }
    };

    auto add_pawn_moves = [&add_if_valid, &from_index, &figure, this, &moves]() {
        if (figure->is_white) {
            {
                CellIndex to_index = {from_index.x - 1, from_index.y};
                if (is_in_range(to_index)) {
                    const std::optional<Figure>& to_figure = chessboard_[to_index.x][to_index.y];
                    if (!to_figure) {
                        moves.push_back({from_index, to_index});
                        to_index = {from_index.x - 2, from_index.y};
                        if (from_index.x == 6 && is_in_range(to_index)) {
                            const std::optional<Figure>& to_figure2 = chessboard_[to_index.x][to_index.y];
                            if (!to_figure2) {
                                moves.push_back({from_index, to_index});
                            }
                        }
                    }
                }
            }
            {
                CellIndex to_index = {from_index.x - 1, from_index.y + 1};
                if (is_in_range(to_index) && chessboard_[to_index.x][to_index.y] &&
                    !chessboard_[to_index.x][to_index.y]->is_white) {
                    add_if_valid(to_index);
                }
            }
            {
                CellIndex to_index = {from_index.x - 1, from_index.y - 1};
                if (is_in_range(to_index) && chessboard_[to_index.x][to_index.y] &&
                    !chessboard_[to_index.x][to_index.y]->is_white) {
                    add_if_valid(to_index);
                }
            }
        } else {
            {
                CellIndex to_index = {from_index.x + 1, from_index.y};
                if (is_in_range(to_index)) {
                    const std::optional<Figure>& to_figure = chessboard_[to_index.x][to_index.y];
                    if (!to_figure) {
                        moves.push_back({from_index, to_index});
                        to_index = {from_index.x + 2, from_index.y};
                        if (from_index.x == 1 && is_in_range(to_index)) {
                            const std::optional<Figure>& to_figure2 = chessboard_[to_index.x][to_index.y];
                            if (!to_figure2) {
                                moves.push_back({from_index, to_index});
                            }
                        }
                    }
                }
            }
            {
                CellIndex to_index = {from_index.x + 1, from_index.y + 1};
                if (is_in_range(to_index) && chessboard_[to_index.x][to_index.y] &&
                    chessboard_[to_index.x][to_index.y]->is_white) {
                    add_if_valid(to_index);
                }
            }
            {
                CellIndex to_index = {from_index.x + 1, from_index.y - 1};
                if (is_in_range(to_index) && chessboard_[to_index.x][to_index.y] &&
                    chessboard_[to_index.x][to_index.y]->is_white) {
                    add_if_valid(to_index);
                }
            }
        }
    };

    switch (figure->figure_name) {
        case FigureName::PAWN: {
            add_pawn_moves();
            break;
        }
        case FigureName::KING: {
            add_king_moves();
            break;
        }
        case FigureName::ROOK: {
            add_rook_moves();
            break;
        }
        case FigureName::KNIGHT: {
            add_knight_moves();
            break;
        }
        case FigureName::BISHOP: {
            add_bishop_moves();
            break;
        }
        case FigureName::QUEEN: {
            add_rook_moves();
            add_bishop_moves();
            break;
        }
    }

    std::sort(moves.begin(), moves.end());
    moves.erase(std::unique(moves.begin(), moves.end()), moves.end());

    return moves;
}
std::vector<Move> Chessboard::get_all_attack_moves(bool is_white) const {
    std::vector<Move> all_valid_moves;
    for (size_t row = 0; row < chessboard_.size(); ++row) {
        for (size_t column = 0; column < chessboard_[row].size(); ++column) {
            CellIndex cell_index = {static_cast<int>(row), static_cast<int>(column)};
            if (!chessboard_[cell_index.x][cell_index.y]) {
                continue;
            }
            if (is_white != chessboard_[cell_index.x][cell_index.y]->is_white) {
                continue;
            }
            std::vector<Move> valid_moves = get_attack_moves(cell_index);
            all_valid_moves.insert(all_valid_moves.end(), valid_moves.begin(), valid_moves.end());
        }
    }
    return all_valid_moves;
}

}  // namespace chess::core
