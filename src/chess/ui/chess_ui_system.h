#pragma once

#include "chess/ui/chess_asset_constants.h"
#include "chess/ui/overlays/imgui_overlay.h"

#include <chess/core/game.h>

#include <engine/core/module.h>
#include <engine/window/image.h>
#include <engine/window/types.h>

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
    void cursor_clicked_callback(float x, float y);
    void cursor_release_callback(float x, float y);

private:
    struct ChessboardUI {
        int board_start_x = -1;
        int board_start_y = -1;
        int cell_x_size = -1;
        int cell_y_size = -1;

        engine::window::WindowPos get_cell(core::CellIndex cell_index) const;
        core::CellIndex get_cell_index(engine::window::WindowPos window_pos) const;
        float get_figure_scale(const engine::window::IImage* figure_image) const;
        engine::window::WindowPos get_figure_start_pos(engine::window::WindowPos cell_pos,
                                                       const engine::window::IImage* figure_image,
                                                       float figure_scale) const;
    };

public:
    static float kCellScaleFactor;
    static int kBorderSizeX;
    static int kBorderSizeY;

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

    void fill_cell(const core::CellIndex& cell_index, int32_t colour);

    std::shared_ptr<core::PlayerI> player1_ = nullptr;
    std::shared_ptr<core::PlayerI> player2_ = nullptr;
    std::shared_ptr<core::ChessGame> game_ = nullptr;
    ChessboardUI chessboard_ui_;
    std::optional<core::CellIndex> hovered_cell_;
    std::optional<core::CellIndex> selected_cell_;
    std::unordered_map<ChessAssetConstants, engine::window::IImage*> asset_to_image_;

    std::optional<engine::window::WindowPos> selected_processing_pos_;

    std::vector<std::unique_ptr<ImGuiOverlay>> overlays;

    friend class DebugMouseOverlay;
};

}  // namespace chess::ui
