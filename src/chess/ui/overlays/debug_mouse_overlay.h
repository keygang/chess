#include "chess/ui/overlays/imgui_overlay.h"
#include "chess/ui/chess_ui.h"

namespace chess::ui {

class DebugMouseOverlay : public ImGuiOverlay {
public:
  explicit DebugMouseOverlay(ChessUI& chess_ui);
  void update() override;
  std::string get_name() const override;

private:
  const ChessUI& chess_ui_;
};

}