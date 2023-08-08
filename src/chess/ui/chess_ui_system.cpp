#include "chess/ui/chess_ui_system.h"
#include "chess/ui/assets_utils.h"
#include "chess/ui/overlays/debug_mouse_overlay.h"
#include "chess/ui/overlays/settings_overlay.h"

#include <engine/window/image_utils.h>

#include <fmt/format.h>
#include <imgui.h>
#include <iostream>
#include <optional>
#include <set>
#include <utility>

namespace chess::ui {

using namespace engine::window;

namespace {

ImU32 kBlackColour = IM_COL32(0, 0, 0, 255);

}  // namespace

float ChessUISystem::kCellScaleFactor = 0.8;
float ChessUISystem::kBorderSizeX = 380;
float ChessUISystem::kBorderSizeY = 380;

ImU32 ChessUISystem::kHoveredCellColour = IM_COL32(0, 0, 0, 100);
ImU32 ChessUISystem::kSelectedCellColour = IM_COL32(0, 255, 0, 100);
ImU32 ChessUISystem::kValidMoveCellColour = IM_COL32(0, 0, 255, 100);

void ChessUISystem::update() {
    update_main_menu_bar();
    update_chessboard_images();
}

void ChessUISystem::update_main_menu_bar() {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("Game##game_menu")) {
            if (ImGui::MenuItem("Reset game")) {
                game_->reset();
            }
            ImGui::EndMenu();
        }

        {
            auto it = std::find_if(overlays.begin(), overlays.end(), [](const std::unique_ptr<ImGuiOverlay>& overlay) {
                return overlay->get_name() == "SettingsOverlay";
            });
            assert(it != overlays.end());
            ImGui::MenuItem("Settings##settings_menu", "", &(*it)->enabled);
        }

        if (ImGui::BeginMenu("debug##debug_menu")) {
            {
                auto it =
                    std::find_if(overlays.begin(), overlays.end(), [](const std::unique_ptr<ImGuiOverlay>& overlay) {
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

void ChessUISystem::update_chessboard_images() {
    draw_chessboard();
    draw_promotion();
}

ChessUISystem::ChessUISystem() {
    load_assets();
    setup_game();
    create_overlays();
}

void ChessUISystem::load_assets() {
    auto load_image_with_assert = [](const std::string& asset_name) -> IImage* {
        IImage* image = ImageUtils::load_image(utils::get_asset_path(asset_name));
        assert(image);
        return image;
    };

    asset_to_image_[ChessAssetConstants::BOARD] = load_image_with_assert("chess_set3/boards/1_480x480.png");

    // Black
    asset_to_image_[ChessAssetConstants::ROOK_BLACK] =
        load_image_with_assert("chess_set2/PNGs/No shadow/1x/b_rook_1x_ns.png");
    asset_to_image_[ChessAssetConstants::BISHOP_BLACK] =
        load_image_with_assert("chess_set2/PNGs/No shadow/1x/b_bishop_1x_ns.png");
    asset_to_image_[ChessAssetConstants::KNIGHT_BLACK] =
        load_image_with_assert("chess_set2/PNGs/No shadow/1x/b_knight_1x_ns.png");
    asset_to_image_[ChessAssetConstants::KING_BLACK] =
        load_image_with_assert("chess_set2/PNGs/No shadow/1x/b_king_1x_ns.png");
    asset_to_image_[ChessAssetConstants::PAWN_BLACK] =
        load_image_with_assert("chess_set2/PNGs/No shadow/1x/b_pawn_1x_ns.png");
    asset_to_image_[ChessAssetConstants::QUEEN_BLACK] =
        load_image_with_assert("chess_set2/PNGs/No shadow/1x/b_queen_1x_ns.png");

    // White
    asset_to_image_[ChessAssetConstants::ROOK_WHITE] =
        load_image_with_assert("chess_set2/PNGs/No shadow/1x/w_rook_1x_ns.png");
    asset_to_image_[ChessAssetConstants::BISHOP_WHITE] =
        load_image_with_assert("chess_set2/PNGs/No shadow/1x/w_bishop_1x_ns.png");
    asset_to_image_[ChessAssetConstants::KNIGHT_WHITE] =
        load_image_with_assert("chess_set2/PNGs/No shadow/1x/w_knight_1x_ns.png");
    asset_to_image_[ChessAssetConstants::KING_WHITE] =
        load_image_with_assert("chess_set2/PNGs/No shadow/1x/w_king_1x_ns.png");
    asset_to_image_[ChessAssetConstants::PAWN_WHITE] =
        load_image_with_assert("chess_set2/PNGs/No shadow/1x/w_pawn_1x_ns.png");
    asset_to_image_[ChessAssetConstants::QUEEN_WHITE] =
        load_image_with_assert("chess_set2/PNGs/No shadow/1x/w_queen_1x_ns.png");
}
void ChessUISystem::draw_chessboard() {
    //  ImVec2 window_pos = ImGui::GetWindowPos();

    const auto* engine = engine::Engine::get_instance();

    IImage* board_image = asset_to_image_[ChessAssetConstants::BOARD];

    const engine::window::Window* window = engine->get_window();
    Window::Size window_size = window->get_size();
    int board_half_width = kBorderSizeX / 2;
    int board_half_height = kBorderSizeY / 2;
    int board_start_x = window_size.width / 2 - board_half_width;
    int board_start_y = window_size.height / 2 - board_half_height;

    ImageUtils::render_image(board_image, board_start_x, board_start_y, kBorderSizeX, kBorderSizeY);

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
//            ImGui::GetForegroundDrawList()->AddText({cell_x, cell_y}, kBlackColour,
//                                                    fmt::format("{},{}", row, column).c_str());
//            ImGui::GetForegroundDrawList()->AddText({cell_x, cell_y + 10}, kBlackColour,
//                                                    fmt::format("{:.0f},{:.0f}", cell_x, cell_y).c_str());

            const std::optional<core::Figure>& chessboard_cell = chessboard[row][column];
            if (!chessboard_cell) {
                continue;
            }
            const core::Figure& figure = *chessboard_cell;
            ChessAssetConstants asset_constant = figure_to_asset_constant(figure);

            if (asset_to_image_.count(asset_constant)) {
                const engine::window::IImage* figure_image = asset_to_image_[asset_constant];
                float scale = std::min(cell_x_size / figure_image->width(), cell_y_size / figure_image->height());
                scale *= kCellScaleFactor;
                float figure_start_x =
                    (cell_x * 2 + chessboard_ui_.cell_x_size) / 2 - figure_image->width() * scale / 2;
                float figure_start_y =
                    (cell_y * 2 + chessboard_ui_.cell_y_size) / 2 - figure_image->height() * scale / 2;
                if (selected_cell_ &&
                    selected_cell_ == core::CellIndex{static_cast<int>(row), static_cast<int>(column)} &&
                    selected_processing_cell_) {
                    ImVec2 cursor_pos = ImGui::GetMousePos();
                    figure_start_x += cursor_pos.x - selected_processing_cell_.value().px;
                    figure_start_y += cursor_pos.y - selected_processing_cell_.value().py;
                }
                ImageUtils::render_image(figure_image, figure_start_x, figure_start_y, scale);
            }
        }
    }

    // Draw boarder labels
    for (size_t i = 0; i < chessboard.size(); ++i) {
        {
            float pos_x = chessboard_ui_.cells[i][0].px - chessboard_ui_.cell_x_size / 2;
            float pos_y = chessboard_ui_.cells[i][0].py;

            ImGui::GetForegroundDrawList()->AddText({pos_x, pos_y}, kBlackColour,
                                                    fmt::format("{}", 1 + chessboard.size() - i - 1).c_str());
        }
        {
            float pos_x = chessboard_ui_.cells.back()[i].px + chessboard_ui_.cell_x_size / 2;
            float pos_y = chessboard_ui_.cells.back()[i].py + chessboard_ui_.cell_y_size * 1.5;

            ImGui::GetForegroundDrawList()->AddText({pos_x, pos_y}, kBlackColour,
                                                    fmt::format("{}", static_cast<char>('A' + i)).c_str());
        }
    }

    update_cursor();

    if (hovered_cell_) {
        fill_cell(*hovered_cell_, kHoveredCellColour);
    }

    if (selected_cell_) {
        assert(game_);
        std::vector<core::Move> valid_moves = game_->get_valid_moves(*selected_cell_);
        if (valid_moves.empty()) {
            selected_cell_ = std::nullopt;
        } else {
            for (const core::Move& move : valid_moves) {
                fill_cell(move.to, kValidMoveCellColour);
            }
            fill_cell(*selected_cell_, kSelectedCellColour);
        }
    }
}

void ChessUISystem::update_chessboard_buttons() {}

void ChessUISystem::cursor_pos_callback(float x, float y) {
    hovered_cell_ = get_cell(x, y);
    const core::Chessboard& chessboard = game_ ? game_->get_chessboard() : core::kDefaultChessboard;
    if (!chessboard.is_in_range(*hovered_cell_)) {
        if (!promotion_ui_) {
            hovered_cell_ = std::nullopt;
            return;
        }

        // TODO:
        static const std::set<core::CellIndex> cells = {
            {2, 8},
            {3, 8},
            {4, 8},
            {5, 8},
        };
        if (!cells.count(*hovered_cell_)) {
            hovered_cell_ = std::nullopt;
            return;
        }
    }
}
void ChessUISystem::cursor_down_callback(float x, float y) {
    if (!game_) {
        return;
    }
    if (selected_processing_cell_) {
        return;
    }
    const core::Chessboard& chessboard = game_ ? game_->get_chessboard() : core::kDefaultChessboard;
    if (selected_cell_) {
        core::CellIndex new_selected_cell = get_cell(x, y);
        if (!chessboard.is_in_range(new_selected_cell)) {
            selected_cell_ = std::nullopt;
            return;
        }
        auto move = core::Move{selected_cell_->x, selected_cell_->y, new_selected_cell.x, new_selected_cell.y};
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
        if (chessboard.is_in_range(*selected_cell_)) {
            const std::optional<core::Figure>& chessboard_cell =
                game_->get_chessboard()[selected_cell_->x][selected_cell_->y];
            if (!chessboard_cell) {
                selected_cell_ = std::nullopt;
                return;
            }
            const core::Figure& figure = *chessboard_cell;
            if ((figure.is_white && !game_->is_white_move()) || (!figure.is_white && game_->is_white_move())) {
                selected_cell_ = std::nullopt;
                return;
            }
            selected_processing_cell_ = {x, y};
            return;
        } else {
            if (!promotion_ui_) {
                selected_cell_ = std::nullopt;
                return;
            }

            // TODO:
            static const std::set<core::CellIndex> cells = {
                {2, 8},
                {3, 8},
                {4, 8},
                {5, 8},
            };
            if (!cells.count(*selected_cell_)) {
                selected_cell_ = std::nullopt;
                return;
            }
            game_->move(core::ChessGame::kValidPromotions[selected_cell_->x - 2]);
            selected_cell_ = std::nullopt;
        }
    }
    if (selected_processing_cell_) {
        if (!game_) {
            return;
        }
        const core::Chessboard& chessboard = game_ ? game_->get_chessboard() : core::kDefaultChessboard;

        core::CellIndex new_selected_cell = get_cell(x, y);
        if (!chessboard.is_in_range(new_selected_cell)) {
            selected_cell_ = std::nullopt;
            return;
        }
        auto move = core::Move{selected_cell_->x, selected_cell_->y, new_selected_cell.x, new_selected_cell.y};
        if (game_->is_valid_move(move)) {
            if (game_->is_white_move()) {
                player1_->move(move);
            } else {
                player2_->move(move);
            }
        }
        selected_cell_ = std::nullopt;
        selected_processing_cell_ = std::nullopt;
    }

    selected_processing_cell_ = {};
}

void ChessUISystem::cursor_release_callback(float x, float y) {
}

core::CellIndex ChessUISystem::get_cell(float x, float y) const {
    int cell_x_index = (x - chessboard_ui_.cells[0][0].px) / chessboard_ui_.cell_x_size;
    int cell_y_index = (y - chessboard_ui_.cells[0][0].py) / chessboard_ui_.cell_y_size;
    return core::CellIndex{cell_y_index, cell_x_index};
}
void ChessUISystem::fill_cell(const core::CellIndex& cell_index, int32_t colour) {
    float cell_start_px = chessboard_ui_.cells[0][0].px + cell_index.y * chessboard_ui_.cell_x_size;
    float cell_start_py = chessboard_ui_.cells[0][0].py + cell_index.x * chessboard_ui_.cell_y_size;
    ImGui::GetForegroundDrawList()->AddRectFilled(
        {cell_start_px, cell_start_py},
        {cell_start_px + chessboard_ui_.cell_x_size, cell_start_py + chessboard_ui_.cell_y_size}, colour);
}
void ChessUISystem::setup_game() {
    game_ = std::make_shared<core::ChessGame>();
    player1_ = std::make_shared<PlayerUI>(game_);
    player2_ = std::make_shared<PlayerUI>(game_);
}
void ChessUISystem::update_cursor() {
    ImVec2 cursor_pos = ImGui::GetMousePos();

    cursor_pos_callback(cursor_pos.x, cursor_pos.y);
    if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
        cursor_down_callback(cursor_pos.x, cursor_pos.y);
    }
    if (!ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
        selected_processing_cell_ = std::nullopt;
    }
}
ChessUISystem::~ChessUISystem() {}

void ChessUISystem::create_overlays() {
    overlays.push_back(std::make_unique<DebugMouseOverlay>(*this));
    overlays.push_back(std::make_unique<SettingsOverlay>(*this));
}
void ChessUISystem::draw_promotion() {
    if (game_ && game_->is_promotion()) {
        float promotion_start_x =
            chessboard_ui_.cells[0][0].px + game_->get_chessboard()[0].size() * chessboard_ui_.cell_x_size;
        float promotion_start_y = chessboard_ui_.cells[0][0].py + 2 * chessboard_ui_.cell_y_size;
        promotion_ui_ = PromotionUI{promotion_start_x, promotion_start_y};

        int i = 0;
        for (const core::FigureName& figure_name : core::ChessGame::kValidPromotions) {
            float cell_x = promotion_start_x;
            float cell_y = promotion_start_y + i * chessboard_ui_.cell_y_size;

            core::Figure figure = {figure_name, game_->is_white_move()};
            ChessAssetConstants asset_constant = figure_to_asset_constant(figure);

            if (asset_to_image_.count(asset_constant)) {
                const engine::window::IImage* figure_image = asset_to_image_[asset_constant];
                float scale = std::min(chessboard_ui_.cell_x_size / figure_image->width(),
                                       chessboard_ui_.cell_y_size / figure_image->height());
                scale *= kCellScaleFactor;
                float figure_start_x =
                    (cell_x * 2 + chessboard_ui_.cell_x_size) / 2 - figure_image->width() * scale / 2;
                float figure_start_y =
                    (cell_y * 2 + chessboard_ui_.cell_y_size) / 2 - figure_image->height() * scale / 2;
                ImageUtils::render_image(figure_image, figure_start_x, figure_start_y, scale);
            }

            i += 1;
        }
    } else {
        promotion_ui_ = std::nullopt;
    }
}

void PlayerUI::move(core::Move move) {
    if (auto game = game_.lock()) {
        game->move(move);
    }
}

PlayerUI::PlayerUI(std::weak_ptr<core::ChessGame> game) : game_(std::move(game)) {}

}  // namespace chess::ui
