#pragma once

#include "chess/ui/image.h"

#include <imgui.h>

namespace ImGui {

void draw_image(const chess::ui::Image* image, int x1, int y1, int x2, int y2) {
    ImGui::GetBackgroundDrawList()->AddImage((void*)(int)image->get_id(),
                                             {static_cast<float>(x1), static_cast<float>(y1)},
                                             {static_cast<float>(x2), static_cast<float>(y2)});
}

void draw_image(const chess::ui::Image* image, int x1, int y1) {
    draw_image(image, x1, y1, x1 + image->get_width(), y1 + image->get_height());
}

void draw_image(const chess::ui::Image* image, int x1, int y1, float scale) {
    draw_image(image, x1, y1, x1 + image->get_width() * scale, y1 + image->get_height() * scale);
}

}  // namespace ImGui