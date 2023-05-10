#include "chess/ui/overlays/settings_overlay.h"

#include <imgui.h>

namespace chess::ui {

SettingsOverlay::SettingsOverlay(ChessUI& chess_ui) : chess_ui_(chess_ui) {}

void SettingsOverlay::update() {
    if (ImGui::Begin("Settings")) {
        if (ImGui::SliderFloat("kCellScaleFactor", &chess::ui::ChessUI::kCellScaleFactor, 0, 1)) {}
        if (ImGui::SliderFloat("kBorderSizeX", &chess::ui::ChessUI::kBorderSizeX, 0, 1000)) {}
        if (ImGui::SliderFloat("kBorderSizeY", &chess::ui::ChessUI::kBorderSizeY, 0, 1000)) {}
    }
    ImGui::End();
}

std::string SettingsOverlay::get_name() const {
    static const std::string name = "SettingsOverlay";
    return name;
}

}  // namespace chess::ui