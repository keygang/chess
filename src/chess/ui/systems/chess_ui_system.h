#pragma once

#include "chess/ui/chess_asset_constants.h"
#include "chess/ui/overlays/imgui_overlay.h"
#include "chess/ui/single_components/chessbard_ui_single_component.h"
#include "chess/ui/single_components/game_state_single_component.h"
#include "chess/ui/single_components/important_cells_single_component.h"

#include <chess/core/game.h>

#include <engine/base/color.h>
#include <engine/core/module.h>
#include <engine/window/image.h>
#include <engine/window/types.h>

#include <imgui.h>

#include <list>
#include <unordered_map>

namespace chess::ui {

class ChessUISystem : public engine::core::System {
public:
    ChessUISystem();
    ~ChessUISystem() override;

    void update() override;

public:
    static int kBorderSizeX;
    static int kBorderSizeY;

    static engine::base::Color kHoveredCellColour;
    static engine::base::Color kSelectedCellColour;
    static engine::base::Color kValidMoveCellColour;

    static engine::base::Color kChessboardColour1;
    static engine::base::Color kChessboardColour2;

private:
    struct SystemContext {
        ImportantCellsSingleComponent* important_cells_sc = nullptr;
        ChessboardUISingleComponent* chessboard_ui_sc = nullptr;
        GameStateSingleComponent* game_state_sc = nullptr;
    };

private:
    void load_assets();
    void setup_game(GameStateSingleComponent* game_ui_sc);
    void create_overlays();

    void draw_main_menu_bar(SystemContext& system_context);
    void draw_chessboard(SystemContext& system_context);
    void draw_figures(SystemContext& system_context);
    void draw_actions(SystemContext& system_context);
    void draw_promotion(SystemContext& system_context);

    static void fill_cell(const ChessboardUISingleComponent* chessboard_ui_sc, const core::CellIndex& cell_index,
                          const engine::base::Color& color);

    std::unordered_map<ChessAssetConstants, engine::window::IImage*> asset_to_image_;
    std::vector<std::unique_ptr<ImGuiOverlay>> overlays;
};

}  // namespace chess::ui
