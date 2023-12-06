#include "chess/core/game.h"

#include <cassert>

namespace chess::core {

const std::vector<FigureName> ChessGame::kValidPromotions = {
    FigureName::KNIGHT,
    FigureName::BISHOP,
    FigureName::ROOK,
    FigureName::QUEEN,
};

bool ChessGame::move(const std::variant<Move, FigureName>& move) {
    if (!is_valid_move(move)) {
        return false;
    }
    push_move(move);

    return true;
}

const Chessboard& ChessGame::get_chessboard() const {
    return chessboard_;
}

ChessGame::ChessGame() : chessboard_(kDefaultChessboard) {}

bool ChessGame::is_white_move() const {
    return moves_number_ % 2 == 0;
}

bool ChessGame::is_valid_move(const std::variant<Move, FigureName>& move) const {
    if (const auto* simple_move = std::get_if<Move>(&move)) {
        return is_valid_simple_move(*simple_move);
    }
    return is_valid_promotion(*std::get_if<FigureName>(&move));
}

void ChessGame::reset() {
    moves_number_ = 0;
    moves_.clear();
    chessboard_ = kDefaultChessboard;
}

std::vector<Move> ChessGame::get_valid_moves(const CellIndex& from_index) const {
    if (!chessboard_.is_in_range(from_index)) {
        return {};
    }
    if (is_promotion_) {
        return {};
    }
    const std::optional<Figure>& figure = chessboard_[from_index];
    if (!figure) {
        return {};
    }

    std::vector<Move> moves = chessboard_.get_attack_moves(from_index);

    auto add_pawn_moves = [&from_index, this, &moves, &figure]() {
        if (figure->is_white) {
            // en passant
            {
                CellIndex to_index = {from_index.x - 1, from_index.y - 1};
                if (chessboard_.is_in_range(to_index) && is_pawn_en_passant({from_index, to_index})) {
                    moves.push_back({from_index, to_index});
                }
            }
            {
                CellIndex to_index = {from_index.x - 1, from_index.y + 1};
                if (chessboard_.is_in_range(to_index) && is_pawn_en_passant({from_index, to_index})) {
                    moves.push_back({from_index, to_index});
                }
            }
        } else {
            {
                CellIndex to_index = {from_index.x + 1, from_index.y - 1};
                if (chessboard_.is_in_range(to_index) && is_pawn_en_passant({from_index, to_index})) {
                    moves.push_back({from_index, to_index});
                }
            }
            {
                CellIndex to_index = {from_index.x + 1, from_index.y + 1};
                if (chessboard_.is_in_range(to_index) && is_pawn_en_passant({from_index, to_index})) {
                    moves.push_back({from_index, to_index});
                }
            }
        }
    };

    auto add_king_moves = [&from_index, this, &moves, &figure]() {
        ChessGame fake_game(*this);

        // TODO: make it const
        CellIndex king_cell_index = {0, 4};
        CellIndex left_rook_index = {0, 0};
        CellIndex right_rook_index = {0, 7};
        if (figure->is_white) {
            king_cell_index = {7, 4};
            left_rook_index = {7, 0};
            right_rook_index = {7, 7};
        }
        if (!is_moved(king_cell_index)) {
            if (fake_game.is_check_move({{0, 0}, {0, 0}})) {
                return;
            }

            if (!is_moved(left_rook_index)) {
                bool is_valid = true;
                for (int to_y = king_cell_index.y - 1; to_y >= king_cell_index.y - 2; --to_y) {
                    CellIndex to = {king_cell_index.x, to_y};
                    if (chessboard_[to] || fake_game.is_check_move({from_index, to})) {
                        is_valid = false;
                        break;
                    }
                }
                if (is_valid) {
                    moves.push_back({from_index, {king_cell_index.x, 2}});
                }
            }
            if (!is_moved(right_rook_index)) {
                bool is_valid = true;
                for (int to_y = king_cell_index.y + 1; to_y <= king_cell_index.y + 2; ++to_y) {
                    CellIndex to = {king_cell_index.x, to_y};
                    if (chessboard_[to] || fake_game.is_check_move({from_index, to})) {
                        is_valid = false;
                        break;
                    }
                }
                if (is_valid) {
                    moves.push_back({from_index, {king_cell_index.x, 6}});
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
        default: {
            break;
        }
    }

    std::sort(moves.begin(), moves.end());
    moves.erase(std::unique(moves.begin(), moves.end()), moves.end());

    ChessGame fake_game(*this);
    for (size_t i = 0; i < moves.size(); ++i) {
        if (fake_game.is_check_move(moves[i])) {
            std::swap(moves[i], moves.back());
            moves.pop_back();
            i -= 1;
        }
    }

    return moves;
}

bool ChessGame::is_pawn_en_passant(const Move& move) const {
    if (moves_.empty()) {
        return false;
    }
    const std::optional<Figure> figure = chessboard_[move.from];
    if (!figure) {
        return false;
    }
    if (figure->figure_name != FigureName::PAWN) {
        return false;
    }
    if (std::get_if<FigureName>(&moves_.back())) {
        return false;
    }
    const Move& last_move = std::get<Move>(moves_.back());
    const std::optional<Figure>& last_figure = chessboard_[last_move.to];
    assert(last_figure);
    if (last_figure->figure_name != FigureName::PAWN) {
        return false;
    }

    if (last_move.to.x != move.from.x || std::abs(last_move.to.y - move.from.y) != 1) {
        return false;
    }

    if (figure->is_white) {
        assert(!last_figure->is_white);
        if (last_move.to.x - last_move.from.x == 2) {
            return CellIndex{move.from.x - 1, last_move.to.y} == move.to;
        }
    } else {
        assert(last_figure->is_white);
        if (last_move.from.x - last_move.to.x == 2) {
            return CellIndex{move.from.x + 1, last_move.to.y} == move.to;
        }
    }

    return false;
}

void ChessGame::pop_move() {
    assert(!moves_.empty());
    std::vector<std::variant<Move, FigureName>> moves = moves_;
    if (std::get_if<FigureName>(&moves.back())) {
        moves.pop_back();
    }
    moves.pop_back();

    reset();
    for (const std::variant<Move, FigureName>& move : moves) {
        push_move(move);
    }
}

void ChessGame::push_move(const std::variant<Move, FigureName>& move) {
    is_promotion_ = false;
    moves_number_ += 1;

    if (const auto* simple_move = std::get_if<Move>(&move)) {
        bool castling = false;
        if (is_pawn_en_passant(*simple_move)) {
            assert(std::get_if<Move>(&moves_.back()));
            chessboard_[std::get<Move>(moves_.back()).to] = std::nullopt;
        } else {
            castling = is_castling(*simple_move);
        }
        std::optional<Figure>& cell = chessboard_[simple_move->from];
        chessboard_[simple_move->to] = cell;
        cell = std::nullopt;
        if (castling) {
            if (simple_move->to.y == 2) {
                std::swap(chessboard_[{simple_move->from.x, simple_move->from.y - 1}],
                          chessboard_[{simple_move->to.x, 0}]);
            } else {
                std::swap(chessboard_[{simple_move->from.x, simple_move->from.y + 1}],
                          chessboard_[{simple_move->to.x, 7}]);
            }
        }

        if (chessboard_[simple_move->to]->figure_name == FigureName::PAWN) {
            if ((chessboard_[simple_move->to]->is_white && simple_move->to.x == 0) ||
                (!chessboard_[simple_move->to]->is_white &&
                 simple_move->to.x + 1 == static_cast<int>(chessboard_.size()))) {
                is_promotion_ = true;
                moves_number_ -= 1;
            }
        }
    } else {
        assert(!moves_.empty());
        assert(std::get_if<Move>(&moves_.back()));
        CellIndex last_cell = std::get<Move>(moves_.back()).to;
        assert(chessboard_[last_cell]);
        chessboard_[last_cell]->figure_name = std::get<FigureName>(move);
    }
    moves_.push_back(move);
}

bool ChessGame::is_check_move(const Move& move) {
    push_move(move);
    std::optional<CellIndex> king_cell_index = chessboard_.find_figure(Figure{FigureName::KING, !is_white_move()});
    assert(king_cell_index);
    std::vector<Move> enemy_moves = chessboard_.get_all_attack_moves(is_white_move());
    if (std::find_if(enemy_moves.begin(), enemy_moves.end(), [&king_cell_index](const Move& enemy_move) {
            return enemy_move.to == king_cell_index;
        }) != enemy_moves.end()) {
        pop_move();
        return true;
    }
    pop_move();
    return false;
}

bool ChessGame::is_moved(const CellIndex& cell_index) const {
    for (const std::variant<Move, FigureName>& move : moves_) {
        if (const auto* simple_move = std::get_if<Move>(&move)) {
            if (simple_move->from == cell_index || simple_move->to == cell_index) {
                return true;
            }
        }
    }
    return false;
}

bool ChessGame::is_castling(const Move& move) const {
    const std::optional<Figure> figure = chessboard_[move.from];
    if (!figure) {
        return false;
    }
    if (figure->figure_name != FigureName::KING) {
        return false;
    }

    return std::abs(move.from.y - move.to.y) == 2;
}

bool ChessGame::is_valid_promotion(FigureName figure_name) const {

    return is_promotion_ &&
           std::find(kValidPromotions.begin(), kValidPromotions.end(), figure_name) != kValidPromotions.end();
}

bool ChessGame::is_valid_simple_move(const Move& move) const {
    if (is_promotion_) {
        return false;
    }

    if (!chessboard_.is_in_range(move)) {
        return false;
    }

    const std::optional<Figure>& figure_from = chessboard_[move.from];
    if (!figure_from) {
        return false;
    }

    if (figure_from->is_white && !is_white_move()) {
        return false;
    }
    if (!figure_from->is_white && is_white_move()) {
        return false;
    }

    std::vector<Move> valid_moves = get_valid_moves(move.from);

    return std::find(valid_moves.begin(), valid_moves.end(), move) != valid_moves.end();
}
bool ChessGame::is_promotion() const {
    return is_promotion_;
}

}  // namespace chess::core
