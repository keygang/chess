#include "chess/ui/chess_ui_module.h"
#include "chess/ui/chess_ui_system.h"

namespace chess::ui {

void ChessUIModule::init() {
    register_system(std::make_unique<ChessUISystem>());
}

void ChessUIModule::term() {}

}  // namespace chess::ui