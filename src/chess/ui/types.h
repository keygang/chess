#pragma once

#include <chess/core/game.h>
#include <chess/core/player.h>

namespace chess::ui {

class PlayerUI : public core::PlayerI {
public:
    explicit PlayerUI(std::weak_ptr<core::ChessGame> game);
    void move(core::Move move) override;

private:
    std::weak_ptr<core::ChessGame> game_;
};

}  // namespace chess::ui
