#pragma once

#include <chess/core/chessboard.h>

#include <engine/core/component.h>
#include <engine/window/image.h>
#include <engine/window/types.h>

#include <set>

namespace chess::ui {

class ChessboardUISingleComponent : public engine::core::Component {
public:
    float cell_scale_factor = 0.8;

    int board_start_x = -1;
    int board_start_y = -1;
    int cell_x_size = -1;
    int cell_y_size = -1;

    bool white_side = true;

    std::vector<core::CellIndex> promotion_choice_cells = {
        {2, 8},
        {3, 8},
        {4, 8},
        {5, 8},
    };

    engine::window::WindowPos get_cell_pos(core::CellIndex cell_index) const;
    core::CellIndex get_cell_index(engine::window::WindowPos window_pos) const;
    float get_figure_scale(const engine::window::IImage* figure_image) const;
    engine::window::WindowPos get_figure_start_pos(core::CellIndex cell_index,
                                                   const engine::window::IImage* figure_image,
                                                   float figure_scale) const;
};

}  // namespace chess::ui
