#pragma once

#include "chess/core/chessboard.h"

#include <engine/core/component.h>
#include <engine/window/types.h>

#include <optional>

namespace chess::ui {

class ImportantCellsSingleComponent : public engine::core::Component {
public:
    std::optional<core::CellIndex> hovered_cell;
    std::optional<core::CellIndex> selected_cell;

    std::optional<engine::window::WindowPos> selected_processing_pos;
};

}  // namespace chess::ui
