#pragma once

#include "chess/ui/chess_asset_constants.h"
#include "chess/ui/image.h"
#include "chess/ui/overlays/imgui_overlay.h"

#include <chess/core/game.h>

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

class ChessUI {
public:
  ChessUI();
  ~ChessUI();

  void update();

  void cursor_pos_callback(float x, float y);
  void cursor_click_callback(float x, float y);

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
  void update_drawable();
  void update_cursor();

  std::optional<core::CellIndex> get_cell(float x, float y) const;

  void fill_cell(const core::CellIndex& cell_index, int32_t colour);

  std::shared_ptr<core::PlayerI> player1_ = nullptr;
  std::shared_ptr<core::PlayerI> player2_ = nullptr;
  std::shared_ptr<core::ChessGame> game_ = nullptr;
  std::list<Image> image_assets_;
  std::unordered_map<ChessAssetConstants, Image*> asset_to_image_;
  ChessboardUI chessboard_ui_;
  std::optional<core::CellIndex> hovered_cell_;
  std::optional<core::CellIndex> selected_cell_;

  std::vector<std::unique_ptr<ImGuiOverlay>> overlays;

  friend class DebugMouseOverlay;
};

}  // namespace chess::ui