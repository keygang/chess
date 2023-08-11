#include "chess/ui/systems/chessboard_input_system.h"

#include <imgui.h>

#include <set>

namespace chess::ui {

ChessboardInputSystem::ChessboardInputSystem() : engine::core::System() {}

void ChessboardInputSystem::update() {
    auto* game_state_sc = world_->get_component_w<GameStateSingleComponent>();
    if (!game_state_sc->game) {
        return;
    }
    auto* important_cells_sc = world_->get_component_w<ImportantCellsSingleComponent>();
    auto* chessboard_ui_sc = world_->get_component_w<ChessboardUISingleComponent>();
    ImVec2 cursor_pos = ImGui::GetMousePos();
    SystemContext context = {.important_cells_sc = important_cells_sc,
                             .chessboard_ui_sc = chessboard_ui_sc,
                             .game_state_sc = game_state_sc,
                             .mouse_pos = {static_cast<int>(cursor_pos.x), static_cast<int>(cursor_pos.y)}};

    cursor_pos_callback(context);
    if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
        cursor_clicked_callback(context);
    }
    if (ImGui::IsMouseReleased(ImGuiMouseButton_Left) && important_cells_sc->selected_processing_pos) {
        cursor_release_callback(context);
        important_cells_sc->selected_processing_pos = std::nullopt;
    }
    if (!ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
        important_cells_sc->selected_processing_pos = std::nullopt;
    }
}

void ChessboardInputSystem::cursor_pos_callback(ChessboardInputSystem::SystemContext& system_context) {
    system_context.important_cells_sc->hovered_cell =
        system_context.chessboard_ui_sc->get_cell_index(system_context.mouse_pos);
    const core::Chessboard& chessboard = system_context.game_state_sc->game->get_chessboard();
    if (!chessboard.is_in_range(*system_context.important_cells_sc->hovered_cell)) {
        if (!system_context.game_state_sc->game->is_promotion()) {
            system_context.important_cells_sc->hovered_cell = std::nullopt;
            return;
        }

        if (std::find(system_context.chessboard_ui_sc->promotion_choice_cells.begin(),
                      system_context.chessboard_ui_sc->promotion_choice_cells.end(),
                      *system_context.important_cells_sc->hovered_cell) ==
            system_context.chessboard_ui_sc->promotion_choice_cells.end()) {
            system_context.important_cells_sc->hovered_cell = std::nullopt;
            return;
        }
    }
}
void ChessboardInputSystem::cursor_clicked_callback(ChessboardInputSystem::SystemContext& system_context) {
    if (!system_context.game_state_sc->game) {
        return;
    }
    if (system_context.important_cells_sc->selected_processing_pos) {
        return;
    }
    if (system_context.important_cells_sc->selected_cell) {
        cursor_release_callback(system_context);
        system_context.important_cells_sc->selected_cell = std::nullopt;
    } else {
        const core::Chessboard& chessboard = system_context.game_state_sc->game->get_chessboard();
        core::CellIndex new_selected_cell = system_context.chessboard_ui_sc->get_cell_index(system_context.mouse_pos);
        system_context.important_cells_sc->selected_cell = new_selected_cell;
        if (chessboard.is_in_range(*system_context.important_cells_sc->selected_cell)) {
            const std::optional<core::Figure>& chessboard_cell =
                chessboard[system_context.important_cells_sc->selected_cell->x]
                          [system_context.important_cells_sc->selected_cell->y];
            if (!chessboard_cell) {
                system_context.important_cells_sc->selected_cell = std::nullopt;
                return;
            }
            const core::Figure& figure = *chessboard_cell;
            if ((figure.is_white && !system_context.game_state_sc->game->is_white_move()) ||
                (!figure.is_white && system_context.game_state_sc->game->is_white_move())) {
                system_context.important_cells_sc->selected_cell = std::nullopt;
                return;
            }
            system_context.important_cells_sc->selected_processing_pos = system_context.mouse_pos;
            return;
        } else {
            if (!system_context.game_state_sc->game->is_promotion()) {
                system_context.important_cells_sc->selected_cell = std::nullopt;
                return;
            }

            if (std::find(system_context.chessboard_ui_sc->promotion_choice_cells.begin(),
                          system_context.chessboard_ui_sc->promotion_choice_cells.end(),
                          *system_context.important_cells_sc->hovered_cell) ==
                system_context.chessboard_ui_sc->promotion_choice_cells.end()) {
                system_context.important_cells_sc->selected_cell = std::nullopt;
                return;
            }
            system_context.game_state_sc->game->move(
                core::ChessGame::kValidPromotions[system_context.important_cells_sc->selected_cell->x - 2]);
            system_context.important_cells_sc->selected_cell = std::nullopt;
        }
    }

    system_context.important_cells_sc->selected_processing_pos = std::nullopt;
}

void ChessboardInputSystem::cursor_release_callback(ChessboardInputSystem::SystemContext& system_context) {
    core::CellIndex new_selected_cell = system_context.chessboard_ui_sc->get_cell_index(system_context.mouse_pos);
    if (system_context.important_cells_sc->selected_cell == new_selected_cell) {
        return;
    }

    auto move = core::Move{*system_context.important_cells_sc->selected_cell, new_selected_cell};
    if (system_context.game_state_sc->game->is_valid_move(move)) {
        if (system_context.game_state_sc->game->is_white_move()) {
            system_context.game_state_sc->player1->move(move);
        } else {
            system_context.game_state_sc->player2->move(move);
        }
    } else {
        system_context.important_cells_sc->selected_cell = std::nullopt;
    }
}

}  // namespace chess::ui
