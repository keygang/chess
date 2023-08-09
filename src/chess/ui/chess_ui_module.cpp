#include "chess/ui/chess_ui_module.h"
#include "chess/ui/systems/chess_ui_system.h"
#include "chess/ui/systems/chessboard_input_system.h"

namespace chess::ui {

void ChessUIModule::init() {
    register_system(std::make_unique<ChessUISystem>());
    register_system(std::make_unique<ChessboardInputSystem>());
}

void ChessUIModule::term() {}

}  // namespace chess::ui