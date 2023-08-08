#include "engine/window/imgui_utils.h"

namespace engine::window {

ImU32 color_to_imgui_color(base::Color color) {
    return ImU32(ImColor(static_cast<int>(color[0] * 255), static_cast<int>(color[1] * 255),
                         static_cast<int>(color[2] * 255), static_cast<int>(color[3] * 255)));
}

}  // namespace engine::window
