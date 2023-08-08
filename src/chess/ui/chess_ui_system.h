#pragma once

#include "chess/ui/chess_asset_constants.h"
#include "chess/ui/overlays/imgui_overlay.h"

#include <chess/core/game.h>

#include <engine/core/module.h>
#include <engine/window/image.h>

#include <imgui.h>

#include <list>
#include <unordered_map>

namespace chess::ui {

class PlayerUI : public core::PlayerI {
public:
    explicit PlayerUI(std::weak_ptr<core::ChessGame> game);
    void move(core::Move move) override;

private:
    std::weak_ptr<core::ChessGame> game_;
};

class ChessUISystem : public engine::core::System {
public:
    ChessUISystem();
    ~ChessUISystem() override;

    void update() override;

    void cursor_pos_callback(float x, float y);
    void cursor_down_callback(float x, float y);
    void cursor_release_callback(float x, float y);

private:
    struct ChessboardUI {
        struct Cell {
            float px = -1;
            float py = -1;
        };

        float cell_x_size = -1;
        float cell_y_size = -1;
        std::array<std::array<Cell, core::kRowsNum>, core::kColumnsNum> cells;
    };

    struct PromotionUI {
        float start_px = -1;
        float start_py = -1;
    };

public:
    static float kCellScaleFactor;
    static float kBorderSizeX;
    static float kBorderSizeY;

    static ImU32 kHoveredCellColour;
    static ImU32 kSelectedCellColour;
    static ImU32 kValidMoveCellColour;

private:
    void load_assets();
    void setup_game();
    void create_overlays();

    void update_main_menu_bar();
    void update_chessboard_images();
    void update_chessboard_buttons();
    void draw_chessboard();
    void draw_promotion();
    void update_cursor();

    core::CellIndex get_cell(float x, float y) const;

    void fill_cell(const core::CellIndex& cell_index, int32_t colour);

    std::shared_ptr<core::PlayerI> player1_ = nullptr;
    std::shared_ptr<core::PlayerI> player2_ = nullptr;
    std::shared_ptr<core::ChessGame> game_ = nullptr;
    ChessboardUI chessboard_ui_;
    std::optional<PromotionUI> promotion_ui_ = std::nullopt;
    std::optional<core::CellIndex> hovered_cell_;
    std::optional<core::CellIndex> selected_cell_;
    std::unordered_map<ChessAssetConstants, engine::window::IImage*> asset_to_image_;

    std::optional<ChessboardUI::Cell> selected_processing_cell_;

    std::vector<std::unique_ptr<ImGuiOverlay>> overlays;

    friend class DebugMouseOverlay;
};

}  // namespace chess::ui
