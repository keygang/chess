#include "chess/ui/chess_ui_system.h"
#include "chess/ui/overlays/imgui_overlay.h"

namespace chess::ui {

class DebugMouseOverlay : public ImGuiOverlay {
public:
    explicit DebugMouseOverlay(ChessUISystem& chess_ui);
    void update() override;
    std::string get_name() const override;

private:
    const ChessUISystem& chess_ui_;
};

}  // namespace chess::ui