#include "chess/ui/overlays/imgui_overlay.h"
#include "chess/ui/systems/chess_ui_system.h"

namespace chess::ui {

class SettingsOverlay : public ImGuiOverlay {
public:
    explicit SettingsOverlay(ChessUISystem& chess_ui);
    void update() override;
    std::string get_name() const override;

private:
    ChessUISystem& chess_ui_;
    ImVec4 background_color_;
};

}  // namespace chess::ui
