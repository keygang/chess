#include "chess/ui/chess_ui_module.h"

#include <engine/base/color.h>
#include <engine/engine.h>

static const int kWindowWidth = 1280;
static const int kWindowHeight = 720;

int main() {
    engine::Engine::Params params;
    params.window_params.window_width = kWindowWidth;
    params.window_params.window_height = kWindowHeight;
    params.window_params.clear_color = engine::base::Color::kBlack;

    auto* engine = engine::Engine::get_instance();
    engine->set_params(params);

    return engine->run(std::make_unique<chess::ui::ChessUIModule>());
}
