#include "chess/ui/types.h"

namespace chess::ui {

void PlayerUI::move(core::Move move) {
    if (auto game = game_.lock()) {
        game->move(move);
    }
}

PlayerUI::PlayerUI(std::weak_ptr<core::ChessGame> game) : game_(std::move(game)) {}

}  // namespace chess::ui
