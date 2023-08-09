#include "chess/ui/overlays/settings_overlay.h"
#include "chess/ui/single_components/chessbard_ui_single_component.h"

#include <engine/engine.h>

#include <imgui.h>

namespace chess::ui {

namespace {

ImVec4 get_current_background_color() {
    engine::Engine* engine = engine::Engine::get_instance();
    engine::Engine::Params params = engine->get_params();
    return {params.window_params.clear_color[0], params.window_params.clear_color[1],
            params.window_params.clear_color[2], params.window_params.clear_color[3] * 255};
}

void set_background_color(ImVec4 color) {
    engine::Engine* engine = engine::Engine::get_instance();
    engine::Engine::Params params = engine->get_params();
    params.window_params.clear_color = engine::base::Color({color.x, color.y, color.z, color.w / 255});
    engine->set_params(params);
}

}  // namespace

SettingsOverlay::SettingsOverlay(ChessUISystem& chess_ui)
    : chess_ui_(chess_ui), background_color_(get_current_background_color()) {}

void SettingsOverlay::update() {
    auto* chessboard_ui_sc = chess_ui_.get_world()->get_component_w<ChessboardUISingleComponent>();
    assert(chessboard_ui_sc);

    if (ImGui::Begin("Settings")) {
        if (ImGui::SliderFloat("cell_scale_factor", &chessboard_ui_sc->cell_scale_factor, 0, 1)) {}
        if (ImGui::SliderInt("kBorderSizeX", &chess::ui::ChessUISystem::kBorderSizeX, 0, 1000)) {}
        if (ImGui::SliderInt("kBorderSizeY", &chess::ui::ChessUISystem::kBorderSizeY, 0, 1000)) {}
        if (ImGui::CollapsingHeader("Background Color")) {
            if (ImGui::ColorPicker4("Background Color pick", &background_color_.x)) {}
            if (ImGui::Button("Apply color")) {
                set_background_color(background_color_);
            }
        }
        if (ImGui::CollapsingHeader("Chess game colors")) {
            if (ImGui::CollapsingHeader("Chessboard color 1")) {
                if (ImGui::ColorPicker4("Chessboard color 1 pick", &chess::ui::ChessUISystem::kChessboardColour1[0])) {}
            }
            if (ImGui::CollapsingHeader("Chessboard color 2")) {
                if (ImGui::ColorPicker4("Chessboard color 2 pick", &chess::ui::ChessUISystem::kChessboardColour2[0])) {}
            }
            if (ImGui::CollapsingHeader("Valid Move Cell Colour")) {
                if (ImGui::ColorPicker4("Valid Move Cell Colour pick",
                                        &chess::ui::ChessUISystem::kValidMoveCellColour[0])) {}
            }
            if (ImGui::CollapsingHeader("Hovered Cell Colour")) {
                if (ImGui::ColorPicker4("Hovered Cell Colour pick", &chess::ui::ChessUISystem::kHoveredCellColour[0])) {
                }
            }
            if (ImGui::CollapsingHeader("Selected Cell Colour")) {
                if (ImGui::ColorPicker4("Selected Cell Colour pick",
                                        &chess::ui::ChessUISystem::kSelectedCellColour[0])) {}
            }
        }
    }
    ImGui::End();
}

std::string SettingsOverlay::get_name() const {
    static const std::string name = "SettingsOverlay";
    return name;
}

}  // namespace chess::ui
