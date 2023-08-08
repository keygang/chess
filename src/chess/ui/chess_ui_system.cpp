#include "chess/ui/chess_ui_system.h"
#include "chess/ui/assets_utils.h"
#include "chess/ui/overlays/debug_mouse_overlay.h"
#include "chess/ui/overlays/settings_overlay.h"

#include <engine/window/image_utils.h>
#include <engine/window/imgui_utils.h>

#include <fmt/format.h>
#include <imgui.h>
#include <optional>
#include <set>
#include <utility>

namespace chess::ui {

using namespace engine::window;

float ChessUISystem::kCellScaleFactor = 0.8;
int ChessUISystem::kBorderSizeX = 580;
int ChessUISystem::kBorderSizeY = 580;

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
    const auto* engine = engine::Engine::get_instance();

    IImage* board_image = asset_to_image_[ChessAssetConstants::BOARD];

    const engine::window::Window* window = engine->get_window();
    Window::Size window_size = window->get_size();
    int board_half_width = kBorderSizeX / 2;
    int board_half_height = kBorderSizeY / 2;
    chessboard_ui_.board_start_x = window_size.width / 2 - board_half_width;
    chessboard_ui_.board_start_y = window_size.height / 2 - board_half_height;

    ImageUtils::render_image(board_image, chessboard_ui_.board_start_x, chessboard_ui_.board_start_y, kBorderSizeX,
                             kBorderSizeY);

    const core::Chessboard& chessboard = game_ ? game_->get_chessboard() : core::kDefaultChessboard;
    chessboard_ui_.cell_x_size = kBorderSizeX / chessboard.size();
    chessboard_ui_.cell_y_size = kBorderSizeY / chessboard[0].size();

    // Draw figures
    for (size_t row = 0; row < chessboard.size(); ++row) {
        for (size_t column = 0; column < chessboard[row].size(); ++column) {
            auto cell_index = core::CellIndex{static_cast<int>(row), static_cast<int>(column)};
            if (selected_cell_ == cell_index) {
                continue;
            }
            engine::window::WindowPos cell = chessboard_ui_.get_cell(cell_index);
            const std::optional<core::Figure>& chessboard_cell = chessboard[row][column];
            if (!chessboard_cell) {
                continue;
            }
            const core::Figure& figure = *chessboard_cell;
            ChessAssetConstants asset_constant = figure_to_asset_constant(figure);

            if (asset_to_image_.count(asset_constant)) {
                const engine::window::IImage* figure_image = asset_to_image_[asset_constant];
                float scale = chessboard_ui_.get_figure_scale(figure_image);
                engine::window::WindowPos figure_start_pos =
                    chessboard_ui_.get_figure_start_pos(cell, figure_image, scale);
                ImageUtils::render_image(figure_image, figure_start_pos.px, figure_start_pos.py, scale);
            }
        }
    }
    if (selected_cell_ && selected_processing_pos_) {
        const std::optional<core::Figure>& chessboard_cell = chessboard[selected_cell_->x][selected_cell_->y];
        if (chessboard_cell) {
            engine::window::WindowPos cell =
                chessboard_ui_.get_cell(core::CellIndex{selected_cell_->x, selected_cell_->y});
            ChessAssetConstants asset_constant = figure_to_asset_constant(*chessboard_cell);
            const engine::window::IImage* figure_image = asset_to_image_[asset_constant];
            float scale = chessboard_ui_.get_figure_scale(figure_image);
            engine::window::WindowPos figure_start_pos = chessboard_ui_.get_figure_start_pos(cell, figure_image, scale);

            ImVec2 cursor_pos = ImGui::GetMousePos();
            figure_start_pos.px += cursor_pos.x - selected_processing_pos_->px;
            figure_start_pos.py += cursor_pos.y - selected_processing_pos_->py;

            ImageUtils::render_image(figure_image, figure_start_pos.px, figure_start_pos.py, scale);
        }
    }

    // Draw boarder labels
    engine::Engine::Params engine_params = engine->get_params();
    engine::base::Color text_color = engine_params.window_params.clear_color.get_inverse();
    for (size_t i = 0; i < chessboard.size(); ++i) {
        {
            auto text_pos = chessboard_ui_.get_cell(core::CellIndex{static_cast<int>(i), -1});
            text_pos.px += chessboard_ui_.cell_x_size / 2;
            text_pos.py += chessboard_ui_.cell_y_size / 2;

            ImGui::GetForegroundDrawList()->AddText({static_cast<float>(text_pos.px), static_cast<float>(text_pos.py)},
                                                    engine::window::color_to_imgui_color(text_color),
                                                    fmt::format("{}", 1 + chessboard.size() - i - 1).c_str());
        }
        {
            auto text_pos =
                chessboard_ui_.get_cell(core::CellIndex{static_cast<int>(chessboard.size()), static_cast<int>(i)});
            text_pos.px += chessboard_ui_.cell_x_size / 2;
            text_pos.py += chessboard_ui_.cell_y_size / 2;

            ImGui::GetForegroundDrawList()->AddText({static_cast<float>(text_pos.px), static_cast<float>(text_pos.py)},
                                                    engine::window::color_to_imgui_color(text_color),
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
    hovered_cell_ = chessboard_ui_.get_cell_index(engine::window::WindowPos{static_cast<int>(x), static_cast<int>(y)});
    const core::Chessboard& chessboard = game_ ? game_->get_chessboard() : core::kDefaultChessboard;
    if (!chessboard.is_in_range(*hovered_cell_)) {
        if (!game_->is_promotion()) {
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
void ChessUISystem::cursor_clicked_callback(float x, float y) {
    if (!game_) {
        return;
    }
    if (selected_processing_pos_) {
        return;
    }
    const core::Chessboard& chessboard = game_ ? game_->get_chessboard() : core::kDefaultChessboard;
    core::CellIndex new_selected_cell =
        chessboard_ui_.get_cell_index(engine::window::WindowPos{static_cast<int>(x), static_cast<int>(y)});
    if (selected_cell_) {
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
        selected_cell_ = new_selected_cell;
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
            selected_processing_pos_ = {static_cast<int>(x), static_cast<int>(y)};
            return;
        } else {
            if (!game_->is_promotion()) {
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
    if (selected_processing_pos_) {
        if (!game_) {
            return;
        }
        const core::Chessboard& chessboard = game_ ? game_->get_chessboard() : core::kDefaultChessboard;

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
        selected_processing_pos_ = std::nullopt;
    }

    selected_processing_pos_ = {};
}

void ChessUISystem::update_cursor() {
    ImVec2 cursor_pos = ImGui::GetMousePos();

    cursor_pos_callback(cursor_pos.x, cursor_pos.y);
    if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
        cursor_clicked_callback(cursor_pos.x, cursor_pos.y);
    }
    if (ImGui::IsMouseReleased(ImGuiMouseButton_Left) && selected_processing_pos_) {
        selected_processing_pos_ = std::nullopt;
        cursor_clicked_callback(cursor_pos.x, cursor_pos.y);
    }
    if (!ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
        selected_processing_pos_ = std::nullopt;
    }
}

void ChessUISystem::cursor_release_callback(float x, float y) {}

void ChessUISystem::fill_cell(const core::CellIndex& cell_index, int32_t colour) {
    engine::window::WindowPos cell = chessboard_ui_.get_cell(cell_index);
    ImGui::GetForegroundDrawList()->AddRectFilled({static_cast<float>(cell.px), static_cast<float>(cell.py)},
                                                  {static_cast<float>(cell.px + chessboard_ui_.cell_x_size),
                                                   static_cast<float>(cell.py + chessboard_ui_.cell_y_size)},
                                                  colour);
}
void ChessUISystem::setup_game() {
    game_ = std::make_shared<core::ChessGame>();
    player1_ = std::make_shared<PlayerUI>(game_);
    player2_ = std::make_shared<PlayerUI>(game_);
}

ChessUISystem::~ChessUISystem() {}

void ChessUISystem::create_overlays() {
    overlays.push_back(std::make_unique<DebugMouseOverlay>(*this));
    overlays.push_back(std::make_unique<SettingsOverlay>(*this));
}

void ChessUISystem::draw_promotion() {
    if (game_ && game_->is_promotion()) {
        int promotion_start_x =
            chessboard_ui_.board_start_x + game_->get_chessboard()[0].size() * chessboard_ui_.cell_x_size;
        int promotion_start_y = chessboard_ui_.board_start_y + 2 * chessboard_ui_.cell_y_size;

        int i = 0;
        for (const core::FigureName& figure_name : core::ChessGame::kValidPromotions) {
            engine::window::WindowPos cell = {promotion_start_x, promotion_start_y + i * chessboard_ui_.cell_y_size};

            core::Figure figure = {figure_name, game_->is_white_move()};
            ChessAssetConstants asset_constant = figure_to_asset_constant(figure);

            if (asset_to_image_.count(asset_constant)) {
                const engine::window::IImage* figure_image = asset_to_image_[asset_constant];
                float scale = chessboard_ui_.get_figure_scale(figure_image);
                engine::window::WindowPos figure_start_pos =
                    chessboard_ui_.get_figure_start_pos(cell, figure_image, scale);
                ImageUtils::render_image(figure_image, figure_start_pos.px, figure_start_pos.py, scale);
            }

            i += 1;
        }
    }
}

void PlayerUI::move(core::Move move) {
    if (auto game = game_.lock()) {
        game->move(move);
    }
}

PlayerUI::PlayerUI(std::weak_ptr<core::ChessGame> game) : game_(std::move(game)) {}

engine::window::WindowPos ChessUISystem::ChessboardUI::get_cell(core::CellIndex cell_index) const {
    int cell_x = board_start_x + cell_index.y * cell_x_size;
    int cell_y = board_start_y + cell_index.x * cell_y_size;
    return engine::window::WindowPos{cell_x, cell_y};
}

core::CellIndex ChessUISystem::ChessboardUI::get_cell_index(engine::window::WindowPos cell) const {
    int cell_x_index = (cell.px - board_start_x) / cell_x_size;
    int cell_y_index = (cell.py - board_start_y) / cell_y_size;
    return core::CellIndex{cell_y_index, cell_x_index};
}
engine::window::WindowPos ChessUISystem::ChessboardUI::get_figure_start_pos(engine::window::WindowPos cell_pos,
                                                                            const engine::window::IImage* figure_image,
                                                                            float figure_scale) const {
    int figure_start_x = (cell_pos.px * 2 + cell_x_size) / 2 - figure_image->width() * figure_scale / 2;
    int figure_start_y = (cell_pos.py * 2 + cell_y_size) / 2 - figure_image->height() * figure_scale / 2;
    return {figure_start_x, figure_start_y};
}

float ChessUISystem::ChessboardUI::get_figure_scale(const engine::window::IImage* figure_image) const {
    float scale = std::min(static_cast<float>(cell_x_size) / figure_image->width(),
                           static_cast<float>(cell_y_size) / figure_image->height());
    scale *= kCellScaleFactor;
    return scale;
}

}  // namespace chess::ui
