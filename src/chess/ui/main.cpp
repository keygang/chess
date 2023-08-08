#include "chess/ui/chess_ui_module.h"

#include <engine/engine.h>

static const int kWindowWidth = 1280;
static const int kWindowHeight = 720;
static const uint32_t kDelayMs = 30000;

int main() {
    engine::Engine::Params params;
    params.window_params.window_width = kWindowWidth;
    params.window_params.window_height = kWindowHeight;

    auto* engine = engine::Engine::get_instance();
    engine->set_params(params);

    return engine->run(std::make_unique<chess::ui::ChessUIModule>());
}