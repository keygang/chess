#include "chess/ui/chess_ui_module.h"
#include "chess/ui/chess_ui.h"

namespace chess::ui {

void ChessUIModule::init() {
    register_system(std::make_unique<ChessUI>());
}

void ChessUIModule::term() {}

}  // namespace chess::ui