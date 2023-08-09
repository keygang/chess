#pragma once

#include <chess/core/game.h>
#include <chess/core/player.h>

#include <engine/core/component.h>

namespace chess::ui {

class GameStateSingleComponent : public engine::core::Component {
public:
    std::shared_ptr<core::PlayerI> player1 = nullptr;
    std::shared_ptr<core::PlayerI> player2 = nullptr;
    std::shared_ptr<core::ChessGame> game = nullptr;
};

}  // namespace chess::ui
