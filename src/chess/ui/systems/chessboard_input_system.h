#pragma once

#include "chess/ui/single_components/chessbard_ui_single_component.h"
#include "chess/ui/single_components/game_state_single_component.h"
#include "chess/ui/single_components/important_cells_single_component.h"

#include <engine/core/system.h>

namespace chess::ui {

class ChessboardInputSystem : public engine::core::System {
public:
    ChessboardInputSystem();

    void update() override;

private:
    struct SystemContext {
        ImportantCellsSingleComponent* important_cells_sc = nullptr;
        ChessboardUISingleComponent* chessboard_ui_sc = nullptr;
        GameStateSingleComponent* game_state_sc = nullptr;
        engine::window::WindowPos mouse_pos;
    };

private:
    void cursor_pos_callback(SystemContext& system_context);
    void cursor_clicked_callback(SystemContext& system_context);
    void cursor_release_callback(SystemContext& system_context);
};

}  // namespace chess::ui
