#include "chess/ui/chess_ui.h"
#include "chess/ui/overlays/imgui_overlay.h"

namespace chess::ui {

class SettingsOverlay : public ImGuiOverlay {
public:
    explicit SettingsOverlay(ChessUI& chess_ui);
    void update() override;
    std::string get_name() const override;

private:
    ChessUI& chess_ui_;
};

}  // namespace chess::ui