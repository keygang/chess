#pragma once

#include "chess/ui/image.h"
#include "chess/ui/chess_asset_constants.h"
#include "chess/ui/imgui_overlay.h"

#include <chess/core/game.h>

#include <list>
#include <unordered_map>

namespace chess::ui {

class DebugMouseOverlay;

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
      float x = -1;
      float y = -1;
    };

    float cell_x_size = -1;
    float cell_y_size = -1;
    std::array<std::array<Cell, core::kRowsNum>, core::kColumnsNum> cells;
  };

  struct CellIndex {
    int row = -1;
    int column = -1;
  };

private:
  void load_assets();
  void setup_game();
  void create_overlays();

  void update_main_menu_bar();
  void update_chessboard_images();
  void update_chessboard_buttons();
  void update_drawable();
  void update_cursor();

  std::optional<CellIndex> get_cell(float x, float y) const;

  void fill_cell(const CellIndex& cell_index, int32_t colour);

  std::shared_ptr<core::PlayerI> player1_ = nullptr;
  std::shared_ptr<core::PlayerI> player2_ = nullptr;
  std::shared_ptr<core::ChessGame> game_ = nullptr;
  std::list<Image> image_assets_;
  std::unordered_map<ChessAssetConstants, Image*> asset_to_image_;
  ChessboardUI chessboard_ui_;
  std::optional<CellIndex> hovered_cell_;
  std::optional<CellIndex> selected_cell_;

  std::vector<std::unique_ptr<ImGuiOverlay>> overlays;

  friend DebugMouseOverlay;
};

class DebugMouseOverlay : public ImGuiOverlay {
public:
  explicit DebugMouseOverlay(ChessUI& chess_ui);
  void update() override;
  std::string get_name() const override;

private:
  const ChessUI& chess_ui_;
};

class SettingsOverlay : public ImGuiOverlay {
public:
  explicit SettingsOverlay(ChessUI& chess_ui);
  void update() override;
  std::string get_name() const override;

private:
  ChessUI& chess_ui_;
};

}  // namespace chess::ui