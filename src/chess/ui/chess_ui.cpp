#include "chess/ui/chess_ui.h"
#include "chess/ui/assets_utils.h"
#include "chess/ui/image.h"
#include "chess/ui/imgui_utils.h"

#include <fmt/format.h>
#include <imgui.h>
#include <iostream>
#include <optional>
#include <utility>

namespace chess::ui {

namespace {

float kCellScaleFactor = 0.8;
float kBorderSizeX = 480;
float kBorderSizeY = 480;

ImU32 kHoveredCellColour = IM_COL32(0, 0, 0, 100);
ImU32 kSelectedCellColour = IM_COL32(0, 255, 0, 100);

ImU32 kBlackColour = IM_COL32(0, 0, 0, 255);

}  // namespace

void ChessUI::update() {
  update_main_menu_bar();
  update_chessboard_images();
}

void ChessUI::update_main_menu_bar() {
  if (ImGui::BeginMainMenuBar()) {
    {
      auto it = std::find_if(overlays.begin(), overlays.end(), [](const std::unique_ptr<ImGuiOverlay>& overlay) {
        return overlay->get_name() == "SettingsOverlay";
      });
      assert(it != overlays.end());
      ImGui::MenuItem("Settings##settings_menu", "", &(*it)->enabled);
    }

    if (ImGui::BeginMenu("debug##debug_menu")) {
      {
        auto it = std::find_if(overlays.begin(), overlays.end(), [](const std::unique_ptr<ImGuiOverlay>& overlay) {
          return overlay->get_name() == "DebugMouseOverlay";
        });
        assert(it != overlays.end());
        ImGui::MenuItem("Debug mouse", "", &(*it)->enabled);
      }
      ImGui::EndMenu();
    }

    ImGui::EndMainMenuBar();
  }

  for (const auto& overlay : overlays) {
    if (overlay->enabled) {
      overlay->update();
    }
  }
}

void ChessUI::update_chessboard_images() {
  update_drawable();
}

ChessUI::ChessUI() {
  load_assets();
  setup_game();
  create_overlays();
}

void ChessUI::load_assets() {
  auto load_image_with_assert = [](const std::string& asset_name) -> Image {
    std::optional<Image> image = utils::load_image_asset(asset_name);
    assert(image);
    return *image;
  };

  auto load_images_with_assert = [](const std::string& asset_name, int rows, int columns) {
    std::optional<std::vector<std::vector<Image>>> images = utils::load_images_asset(asset_name, rows, columns);
    assert(images);
    return *images;
  };

  auto add_images = [this](const auto& images) {
    for (size_t i = 0; i < images.size(); ++i) {
      for (size_t j = 0; j < images[i].size(); ++j) {
        image_assets_.push_back(images[i][j]);
      }
    }
  };

  {
    image_assets_.push_back(load_image_with_assert("chess_set3/boards/1_480x480.png"));
    asset_to_image_[ChessAssetConstants::BOARD] = &image_assets_.back();
  }

  // Black
  {
    image_assets_.push_back(load_image_with_assert("chess_set2/PNGs/No shadow/1x/b_rook_1x_ns.png"));
    asset_to_image_[ChessAssetConstants::ROOK_BLACK] = &image_assets_.back();
  }

  {
    image_assets_.push_back(load_image_with_assert("chess_set2/PNGs/No shadow/1x/b_bishop_1x_ns.png"));
    asset_to_image_[ChessAssetConstants::BISHOP_BLACK] = &image_assets_.back();
  }

  {
    image_assets_.push_back(load_image_with_assert("chess_set2/PNGs/No shadow/1x/b_knight_1x_ns.png"));
    asset_to_image_[ChessAssetConstants::KNIGHT_BLACK] = &image_assets_.back();
  }

  {
    image_assets_.push_back(load_image_with_assert("chess_set2/PNGs/No shadow/1x/b_king_1x_ns.png"));
    asset_to_image_[ChessAssetConstants::KING_BLACK] = &image_assets_.back();
  }

  {
    image_assets_.push_back(load_image_with_assert("chess_set2/PNGs/No shadow/1x/b_pawn_1x_ns.png"));
    asset_to_image_[ChessAssetConstants::PAWN_BLACK] = &image_assets_.back();
  }

  {
    image_assets_.push_back(load_image_with_assert("chess_set2/PNGs/No shadow/1x/b_queen_1x_ns.png"));
    asset_to_image_[ChessAssetConstants::QUEEN_BLACK] = &image_assets_.back();
  }

  // White
  {
    image_assets_.push_back(load_image_with_assert("chess_set2/PNGs/No shadow/1x/w_rook_1x_ns.png"));
    asset_to_image_[ChessAssetConstants::ROOK_WHITE] = &image_assets_.back();
  }

  {
    image_assets_.push_back(load_image_with_assert("chess_set2/PNGs/No shadow/1x/w_bishop_1x_ns.png"));
    asset_to_image_[ChessAssetConstants::BISHOP_WHITE] = &image_assets_.back();
  }

  {
    image_assets_.push_back(load_image_with_assert("chess_set2/PNGs/No shadow/1x/w_knight_1x_ns.png"));
    asset_to_image_[ChessAssetConstants::KNIGHT_WHITE] = &image_assets_.back();
  }

  {
    image_assets_.push_back(load_image_with_assert("chess_set2/PNGs/No shadow/1x/w_king_1x_ns.png"));
    asset_to_image_[ChessAssetConstants::KING_WHITE] = &image_assets_.back();
  }

  {
    image_assets_.push_back(load_image_with_assert("chess_set2/PNGs/No shadow/1x/w_pawn_1x_ns.png"));
    asset_to_image_[ChessAssetConstants::PAWN_WHITE] = &image_assets_.back();
  }

  {
    image_assets_.push_back(load_image_with_assert("chess_set2/PNGs/No shadow/1x/w_queen_1x_ns.png"));
    asset_to_image_[ChessAssetConstants::QUEEN_WHITE] = &image_assets_.back();
  }
}
void ChessUI::update_drawable() {
  ImVec2 window_pos = ImGui::GetWindowPos();
  ImVec2 window_size = ImGui::GetIO().DisplaySize;
  ImVec2 window_center = ImVec2((window_pos.x + window_size.x) / 2, (window_pos.y + window_size.y) / 2);
  ImGui::GetForegroundDrawList()->AddCircle(window_center, 10, IM_COL32(0, 255, 0, 200), 0, 10);

  Image* board_image = asset_to_image_[ChessAssetConstants::BOARD];
  int board_half_width = kBorderSizeX / 2;
  int board_half_height = kBorderSizeY / 2;
  int board_start_x = window_center.x - board_half_width;
  int board_start_y = window_center.y - board_half_height;
  ImGui::draw_image(board_image, board_start_x, board_start_y, board_start_x + kBorderSizeX,
                    board_start_y + kBorderSizeY);

  const core::Chessboard& chessboard = game_ ? game_->get_chessboard() : core::kDefaultChessboard;
  float cell_x_size = kBorderSizeX / chessboard.size();
  float cell_y_size = kBorderSizeY / chessboard[0].size();
  chessboard_ui_.cell_x_size = cell_x_size;
  chessboard_ui_.cell_y_size = cell_y_size;

  for (size_t row = 0; row < chessboard.size(); ++row) {
    for (size_t column = 0; column < chessboard[row].size(); ++column) {
      float cell_x = board_start_x + column * cell_x_size;
      float cell_y = board_start_y + row * cell_y_size;

      chessboard_ui_.cells[row][column] = ChessboardUI::Cell{cell_x, cell_y};
      ImGui::GetForegroundDrawList()->AddText({cell_x, cell_y}, kBlackColour,
                                              fmt::format("{},{}", row, column).c_str());
      ImGui::GetForegroundDrawList()->AddText({cell_x, cell_y + 10}, kBlackColour,
                                              fmt::format("{:.1f},{:.1f}", cell_x, cell_y).c_str());

      const core::ChessboardCell& chessboard_cell = chessboard[row][column];
      if (!chessboard_cell) {
        continue;
      }
      const core::Figure& figure = *chessboard_cell;
      ChessAssetConstants asset_constant = figure_to_asset_constant(figure);

      if (asset_to_image_.count(asset_constant)) {
        Image* figure_image = asset_to_image_[asset_constant];
        float scale = std::min(cell_x_size / figure_image->get_width(), cell_y_size / figure_image->get_height());
        scale *= kCellScaleFactor;
        float figure_start_x = (cell_x * 2 + cell_x_size) / 2 - figure_image->get_width() * scale / 2;
        float figure_start_y = (cell_y * 2 + cell_y_size) / 2 - figure_image->get_height() * scale / 2;
        ImGui::draw_image(figure_image, figure_start_x, figure_start_y, scale);
      }
    }
  }

  // Draw boarder labels
  for (size_t i = 0; i < chessboard.size(); ++i) {
    {
      float pos_x = chessboard_ui_.cells[i][0].x - chessboard_ui_.cell_x_size / 2;
      float pos_y = chessboard_ui_.cells[i][0].y;

      ImGui::GetForegroundDrawList()->AddText({pos_x, pos_y}, kBlackColour,
                                              fmt::format("{}", 1 + chessboard.size() - i - 1).c_str());
    }
    {
      float pos_x = chessboard_ui_.cells.back()[i].x + chessboard_ui_.cell_x_size / 2;
      float pos_y = chessboard_ui_.cells.back()[i].y + chessboard_ui_.cell_y_size * 1.5;

      ImGui::GetForegroundDrawList()->AddText({pos_x, pos_y}, kBlackColour, fmt::format("{}", 1 + i).c_str());
    }
  }

  update_cursor();

  if (hovered_cell_) {
    fill_cell(*hovered_cell_, kHoveredCellColour);
  }
  if (selected_cell_) {
    fill_cell(*selected_cell_, kSelectedCellColour);
  }
}

void ChessUI::update_chessboard_buttons() {}

void ChessUI::cursor_pos_callback(float x, float y) {
  hovered_cell_ = get_cell(x, y);
}
void ChessUI::cursor_click_callback(float x, float y) {
  if (!game_) {
    return;
  }
  if (selected_cell_) {
    std::optional<CellIndex> new_selected_cell = get_cell(x, y);
    if (!new_selected_cell) {
      selected_cell_ = std::nullopt;
      return;
    }
    auto move =
        core::Move{selected_cell_->row, selected_cell_->column, new_selected_cell->row, new_selected_cell->column};
    if (game_->is_valid_move(move)) {
      if (game_->is_white_move()) {
        player1_->move(move);
      } else {
        player2_->move(move);
      }
    }
    selected_cell_ = std::nullopt;
  } else {
    selected_cell_ = get_cell(x, y);
    if (selected_cell_) {
      const core::ChessboardCell& chessboard_cell =
          game_->get_chessboard()[selected_cell_->row][selected_cell_->column];
      if (!chessboard_cell) {
        selected_cell_ = std::nullopt;
        return;
      }
      const core::Figure& figure = *chessboard_cell;
      if ((figure.is_white && !game_->is_white_move()) || (!figure.is_white && game_->is_white_move())) {
        selected_cell_ = std::nullopt;
      }
    }
  }
}

std::optional<ChessUI::CellIndex> ChessUI::get_cell(float x, float y) const {
  if (x < chessboard_ui_.cells[0][0].x || x > chessboard_ui_.cells[0].back().x + chessboard_ui_.cell_x_size ||
      y < chessboard_ui_.cells[0][0].y || y > chessboard_ui_.cells.back()[0].y + chessboard_ui_.cell_y_size) {
    return std::nullopt;
  }
  int cell_x_index = (x - chessboard_ui_.cells[0][0].x) / chessboard_ui_.cell_x_size;
  int cell_y_index = (y - chessboard_ui_.cells[0][0].y) / chessboard_ui_.cell_y_size;
  return CellIndex{cell_y_index, cell_x_index};
}
void ChessUI::fill_cell(const ChessUI::CellIndex& cell_index, int32_t colour) {
  ImGui::GetForegroundDrawList()->AddRectFilled(
      {chessboard_ui_.cells[cell_index.row][cell_index.column].x,
       chessboard_ui_.cells[cell_index.row][cell_index.column].y},
      {chessboard_ui_.cells[cell_index.row][cell_index.column].x + chessboard_ui_.cell_x_size,
       chessboard_ui_.cells[cell_index.row][cell_index.column].y + chessboard_ui_.cell_y_size},
      colour);
}
void ChessUI::setup_game() {
  game_ = std::make_shared<core::ChessGame>();
  player1_ = std::make_shared<PlayerUI>(game_);
  player2_ = std::make_shared<PlayerUI>(game_);
}
void ChessUI::update_cursor() {
  ImVec2 cursor_pos = ImGui::GetMousePos();

  cursor_pos_callback(cursor_pos.x, cursor_pos.y);
  if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
    cursor_click_callback(cursor_pos.x, cursor_pos.y);
  }
}
ChessUI::~ChessUI() {}

void ChessUI::create_overlays() {
  overlays.push_back(std::make_unique<DebugMouseOverlay>(*this));
  overlays.push_back(std::make_unique<SettingsOverlay>(*this));
}

void PlayerUI::move(core::Move move) {
  if (auto game = game_.lock()) {
    game->move(move);
  }
}

PlayerUI::PlayerUI(std::weak_ptr<core::ChessGame> game) : game_(std::move(game)) {}
DebugMouseOverlay::DebugMouseOverlay(ChessUI& chess_ui) : chess_ui_(chess_ui) {}

void DebugMouseOverlay::update() {
  if (ImGui::Begin("Debug mouse")) {
    if (chess_ui_.hovered_cell_) {
      ImGui::Text("%s", fmt::format("Hovered cell: row={}; column={}", chess_ui_.hovered_cell_->row,
                                    chess_ui_.hovered_cell_->column)
                            .c_str());
    } else {
      ImGui::Text("Hovered cell: None");
    }

    if (chess_ui_.selected_cell_) {
      ImGui::Text("%s", fmt::format("Selected cell: row={}; column={}", chess_ui_.selected_cell_->row,
                                    chess_ui_.selected_cell_->column)
                            .c_str());
    } else {
      ImGui::Text("Selected cell: None");
    }
  }
  ImGui::End();
}
std::string DebugMouseOverlay::get_name() const {
  static const std::string name = "DebugMouseOverlay";
  return name;
}
SettingsOverlay::SettingsOverlay(ChessUI& chess_ui) : chess_ui_(chess_ui) {}

void SettingsOverlay::update() {
  if (ImGui::Begin("Settings")) {
    if (ImGui::SliderFloat("kCellScaleFactor", &kCellScaleFactor, 0, 1)) {}
    if (ImGui::SliderFloat("kBorderSizeX", &kBorderSizeX, 0, 1000)) {}
    if (ImGui::SliderFloat("kBorderSizeY", &kBorderSizeY, 0, 1000)) {}
  }
  ImGui::End();
}
std::string SettingsOverlay::get_name() const {
  static const std::string name = "SettingsOverlay";
  return name;
}

}  // namespace chess::ui
