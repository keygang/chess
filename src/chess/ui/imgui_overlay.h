#pragma once

namespace chess::ui {

class ImGuiOverlay {
public:
  ~ImGuiOverlay() = default;
  virtual void update() = 0;
  virtual std::string get_name() const = 0;

  bool enabled = false;
};

}