#include "chess/ui/overlays/debug_mouse_overlay.h"

#include <imgui.h>
#include <fmt/format.h>

namespace chess::ui {

DebugMouseOverlay::DebugMouseOverlay(ChessUI& chess_ui) : chess_ui_(chess_ui) {}

void DebugMouseOverlay::update() {
  if (ImGui::Begin("Debug mouse")) {
    if (chess_ui_.hovered_cell_) {
      ImGui::Text("%s", fmt::format("Hovered cell: row={}; column={}", chess_ui_.hovered_cell_->x,
                                    chess_ui_.hovered_cell_->y)
                            .c_str());
    } else {
      ImGui::Text("Hovered cell: None");
    }

    if (chess_ui_.selected_cell_) {
      ImGui::Text("%s", fmt::format("Selected cell: row={}; column={}", chess_ui_.selected_cell_->x,
                                    chess_ui_.selected_cell_->y)
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

}