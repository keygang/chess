#pragma once

#include <string>

namespace chess::ui {

class ImGuiOverlay {
public:
    virtual ~ImGuiOverlay() = default;
    virtual void update() = 0;
    virtual std::string get_name() const = 0;

    bool enabled = false;
};

}  // namespace chess::ui