#include "chess/ui/single_components/chessbard_ui_single_component.h"

#include <engine/window/image.h>

namespace chess::ui {

engine::window::WindowPos ChessboardUISingleComponent::get_cell_pos(core::CellIndex cell_index) const {
    if (!white_side) {
        cell_index.x = 8 - cell_index.x - 1;
    }
    int cell_x = board_start_x + cell_index.y * cell_x_size;
    int cell_y = board_start_y + cell_index.x * cell_y_size;
    return engine::window::WindowPos{cell_x, cell_y};
}

core::CellIndex ChessboardUISingleComponent::get_cell_index(engine::window::WindowPos cell) const {
    int cell_x_index = (cell.px - board_start_x) / cell_x_size;
    int cell_y_index = (cell.py - board_start_y) / cell_y_size;
    if (!white_side) {
        cell_y_index = 8 - cell_y_index - 1;
    }
    return core::CellIndex{cell_y_index, cell_x_index};
}

engine::window::WindowPos ChessboardUISingleComponent::get_figure_start_pos(core::CellIndex cell_index,
                                                                            const engine::window::IImage* figure_image,
                                                                            float figure_scale) const {
    engine::window::WindowPos cell_pos = get_cell_pos(cell_index);
    int figure_start_x = (cell_pos.px * 2 + cell_x_size) / 2 - figure_image->width() * figure_scale / 2;
    int figure_start_y = (cell_pos.py * 2 + cell_y_size) / 2 - figure_image->height() * figure_scale / 2;
    return {figure_start_x, figure_start_y};
}

float ChessboardUISingleComponent::get_figure_scale(const engine::window::IImage* figure_image) const {
    float scale = std::min(static_cast<float>(cell_x_size) / figure_image->width(),
                           static_cast<float>(cell_y_size) / figure_image->height());
    scale *= cell_scale_factor;
    return scale;
}

}  // namespace chess::ui
