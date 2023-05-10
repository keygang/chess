#pragma once

#include <chess/core/chessboard.h>

#include <array>

namespace chess::ui {

class ChessboardUI {
public:
    struct Cell {
        float px = -1;
        float py = -1;
    };

    float cell_x_size = -1;
    float cell_y_size = -1;

private:
    std::array<std::array<Cell, core::kRowsNum>, core::kColumnsNum> cells;
};

}  // namespace chess::ui