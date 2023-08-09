#include "chess/ui/systems/chess_ui_system.h"
#include "chess/ui/overlays/settings_overlay.h"
#include "chess/ui/single_components/chessbard_ui_single_component.h"
#include "chess/ui/single_components/important_cells_single_component.h"
#include "chess/ui/types.h"
#include "chess/ui/utils/assets_utils.h"

#include <engine/window/image_utils.h>
#include <engine/window/imgui_utils.h>

#include <fmt/format.h>
#include <imgui.h>
#include <optional>
#include <set>
#include <utility>

namespace chess::ui {

using namespace engine::window;

int ChessUISystem::kBorderSizeX = 580;
int ChessUISystem::kBorderSizeY = 580;

engine::base::Color ChessUISystem::kHoveredCellColour = engine::base::Color({0, 0, 0, 0.33});
engine::base::Color ChessUISystem::kSelectedCellColour = engine::base::Color({0, 1, 0, 0.33});
engine::base::Color ChessUISystem::kValidMoveCellColour = engine::base::Color({0, 0, 1, 0.33});
engine::base::Color ChessUISystem::kChessboardColour1 = engine::base::Color({181. / 255, 135. / 255, 97. / 255, 1});
engine::base::Color ChessUISystem::kChessboardColour2 = engine::base::Color({239. / 255, 218. / 255, 179. / 255, 1});

void ChessUISystem::update() {
    auto* important_cells_sc = world_->get_component_w<ImportantCellsSingleComponent>();
    auto* chessboard_ui_sc = world_->get_component_w<ChessboardUISingleComponent>();
    auto* game_state_sc = world_->get_component_w<GameStateSingleComponent>();

    SystemContext context = {
        .important_cells_sc = important_cells_sc, .chessboard_ui_sc = chessboard_ui_sc, .game_state_sc = game_state_sc};

    draw_main_menu_bar(context);
    draw_chessboard(context);
    draw_actions(context);
    draw_figures(context);
    draw_promotion(context);
}

void ChessUISystem::draw_main_menu_bar(SystemContext& system_context) {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("Game##system_context.game_state_sc->game_menu")) {
            if (ImGui::MenuItem("Reset game")) {
                system_context.game_state_sc->game->reset();
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

        ImGui::EndMainMenuBar();
    }

    // TODO: move overlays
    for (const auto& overlay : overlays) {
        if (overlay->enabled) {
            overlay->update();
        }
    }
}

ChessUISystem::ChessUISystem() : engine::core::System() {
    world_->add_component<ImportantCellsSingleComponent>();
    world_->add_component<ChessboardUISingleComponent>();
    auto* game_state_sc = world_->add_component<GameStateSingleComponent>();

    load_assets();
    setup_game(game_state_sc);
    create_overlays();
}

void ChessUISystem::load_assets() {
    auto load_image_with_assert = [](const std::string& asset_name) -> IImage* {
        IImage* image = ImageUtils::load_image(utils::get_asset_path(asset_name));
        assert(image);
        return image;
    };

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

void ChessUISystem::draw_chessboard(ChessUISystem::SystemContext& system_context) {
    const auto* engine = engine::Engine::get_instance();

    const engine::window::Window* window = engine->get_window();
    Window::Size window_size = window->get_size();
    int board_half_width = kBorderSizeX / 2;
    int board_half_height = kBorderSizeY / 2;
    system_context.chessboard_ui_sc->board_start_x = window_size.width / 2 - board_half_width;
    system_context.chessboard_ui_sc->board_start_y = window_size.height / 2 - board_half_height;

    const core::Chessboard& chessboard = system_context.game_state_sc->game
                                             ? system_context.game_state_sc->game->get_chessboard()
                                             : core::kDefaultChessboard;
    system_context.chessboard_ui_sc->cell_x_size = kBorderSizeX / chessboard.size();
    system_context.chessboard_ui_sc->cell_y_size = kBorderSizeY / chessboard[0].size();

    for (size_t row = 0; row < chessboard.size(); ++row) {
        for (size_t column = 0; column < chessboard[row].size(); ++column) {
            auto cell_index = core::CellIndex{static_cast<int>(row), static_cast<int>(column)};
            fill_cell(system_context.chessboard_ui_sc, cell_index,
                      (column + row % 2) % 2 ? kChessboardColour1 : kChessboardColour2);
        }
    }

    // Draw boarder labels TODO: rewrite
    engine::Engine::Params engine_params = engine->get_params();
    engine::base::Color text_color = engine_params.window_params.clear_color.get_inverse();
    for (size_t i = 0; i < chessboard.size(); ++i) {
        {
            auto text_pos = system_context.chessboard_ui_sc->get_cell(core::CellIndex{static_cast<int>(i), -1});
            text_pos.px += system_context.chessboard_ui_sc->cell_x_size / 2;
            text_pos.py += system_context.chessboard_ui_sc->cell_y_size / 2;

            ImGui::GetForegroundDrawList()->AddText({static_cast<float>(text_pos.px), static_cast<float>(text_pos.py)},
                                                    engine::window::color_to_imgui_color(text_color),
                                                    fmt::format("{}", 1 + chessboard.size() - i - 1).c_str());
        }
        {
            auto text_pos = system_context.chessboard_ui_sc->get_cell(
                core::CellIndex{static_cast<int>(chessboard.size()), static_cast<int>(i)});
            text_pos.px += system_context.chessboard_ui_sc->cell_x_size / 2;
            text_pos.py += system_context.chessboard_ui_sc->cell_y_size / 2;

            ImGui::GetForegroundDrawList()->AddText({static_cast<float>(text_pos.px), static_cast<float>(text_pos.py)},
                                                    engine::window::color_to_imgui_color(text_color),
                                                    fmt::format("{}", static_cast<char>('A' + i)).c_str());
        }
    }
}

void ChessUISystem::fill_cell(const ChessboardUISingleComponent* chessboard_ui_sc, const core::CellIndex& cell_index,
                              const engine::base::Color& color) {
    engine::window::WindowPos cell = chessboard_ui_sc->get_cell(cell_index);
    engine::Engine::get_instance()->get_manager<Renderer>()->render(
        engine::base::Rect<int>{cell.px, cell.py, chessboard_ui_sc->cell_x_size, chessboard_ui_sc->cell_y_size}, color);
}

void ChessUISystem::setup_game(GameStateSingleComponent* game_state_sc) {
    game_state_sc->game = std::make_shared<core::ChessGame>();
    game_state_sc->player1 = std::make_shared<PlayerUI>(game_state_sc->game);
    game_state_sc->player2 = std::make_shared<PlayerUI>(game_state_sc->game);
}

ChessUISystem::~ChessUISystem() = default;

void ChessUISystem::create_overlays() {
    overlays.push_back(std::make_unique<SettingsOverlay>(*this));
}

void ChessUISystem::draw_promotion(ChessUISystem::SystemContext& system_context) {
    if (system_context.game_state_sc->game && system_context.game_state_sc->game->is_promotion()) {
        int promotion_start_x = system_context.chessboard_ui_sc->board_start_x +
                                system_context.game_state_sc->game->get_chessboard()[0].size() *
                                    system_context.chessboard_ui_sc->cell_x_size;
        int promotion_start_y =
            system_context.chessboard_ui_sc->board_start_y + 2 * system_context.chessboard_ui_sc->cell_y_size;

        int i = 0;
        for (const core::FigureName& figure_name : core::ChessGame::kValidPromotions) {
            engine::window::WindowPos cell = {promotion_start_x,
                                              promotion_start_y + i * system_context.chessboard_ui_sc->cell_y_size};

            core::Figure figure = {figure_name, system_context.game_state_sc->game->is_white_move()};
            ChessAssetConstants asset_constant = figure_to_asset_constant(figure);

            if (asset_to_image_.count(asset_constant)) {
                const engine::window::IImage* figure_image = asset_to_image_[asset_constant];
                float scale = system_context.chessboard_ui_sc->get_figure_scale(figure_image);
                engine::window::WindowPos figure_start_pos =
                    system_context.chessboard_ui_sc->get_figure_start_pos(cell, figure_image, scale);
                ImageUtils::render_image(figure_image, figure_start_pos.px, figure_start_pos.py, scale);
            }

            i += 1;
        }
    }
}

void ChessUISystem::draw_figures(ChessUISystem::SystemContext& system_context) {
    const core::Chessboard& chessboard = system_context.game_state_sc->game
                                             ? system_context.game_state_sc->game->get_chessboard()
                                             : core::kDefaultChessboard;
    // Draw figures
    for (size_t row = 0; row < chessboard.size(); ++row) {
        for (size_t column = 0; column < chessboard[row].size(); ++column) {
            auto cell_index = core::CellIndex{static_cast<int>(row), static_cast<int>(column)};
            if (system_context.important_cells_sc->selected_cell == cell_index &&
                system_context.important_cells_sc->selected_processing_pos) {
                continue;
            }
            engine::window::WindowPos cell = system_context.chessboard_ui_sc->get_cell(cell_index);
            const std::optional<core::Figure>& chessboard_cell = chessboard[row][column];
            if (!chessboard_cell) {
                continue;
            }
            const core::Figure& figure = *chessboard_cell;
            ChessAssetConstants asset_constant = figure_to_asset_constant(figure);

            if (asset_to_image_.count(asset_constant)) {
                const engine::window::IImage* figure_image = asset_to_image_[asset_constant];
                float scale = system_context.chessboard_ui_sc->get_figure_scale(figure_image);
                engine::window::WindowPos figure_start_pos =
                    system_context.chessboard_ui_sc->get_figure_start_pos(cell, figure_image, scale);
                ImageUtils::render_image(figure_image, figure_start_pos.px, figure_start_pos.py, scale);
            }
        }
    }

    if (system_context.important_cells_sc->selected_cell &&
        system_context.important_cells_sc->selected_processing_pos) {
        const std::optional<core::Figure>& chessboard_cell =
            chessboard[*system_context.important_cells_sc->selected_cell];
        if (chessboard_cell) {
            engine::window::WindowPos cell =
                system_context.chessboard_ui_sc->get_cell(*system_context.important_cells_sc->selected_cell);
            ChessAssetConstants asset_constant = figure_to_asset_constant(*chessboard_cell);
            const engine::window::IImage* figure_image = asset_to_image_[asset_constant];
            float scale = system_context.chessboard_ui_sc->get_figure_scale(figure_image);
            engine::window::WindowPos figure_start_pos =
                system_context.chessboard_ui_sc->get_figure_start_pos(cell, figure_image, scale);

            ImVec2 cursor_pos = ImGui::GetMousePos();
            figure_start_pos.px += cursor_pos.x - system_context.important_cells_sc->selected_processing_pos->px;
            figure_start_pos.py += cursor_pos.y - system_context.important_cells_sc->selected_processing_pos->py;

            ImageUtils::render_image(figure_image, figure_start_pos.px, figure_start_pos.py, scale);
        }
    }
}
void ChessUISystem::draw_actions(ChessUISystem::SystemContext& system_context) {
    if (system_context.important_cells_sc->hovered_cell) {
        fill_cell(system_context.chessboard_ui_sc, *system_context.important_cells_sc->hovered_cell,
                  kHoveredCellColour);
    }

    if (system_context.important_cells_sc->selected_cell) {
        assert(system_context.game_state_sc->game);
        std::vector<core::Move> valid_moves =
            system_context.game_state_sc->game->get_valid_moves(*system_context.important_cells_sc->selected_cell);
        for (const core::Move& move : valid_moves) {
            fill_cell(system_context.chessboard_ui_sc, move.to, kValidMoveCellColour);
        }
        fill_cell(system_context.chessboard_ui_sc, *system_context.important_cells_sc->selected_cell,
                  kSelectedCellColour);
    }
}

}  // namespace chess::ui
