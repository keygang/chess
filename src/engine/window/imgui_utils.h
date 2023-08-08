#pragma once

#include "engine/base/color.h"

#include <imgui.h>

namespace engine::window {

ImU32 color_to_imgui_color(base::Color color);

}