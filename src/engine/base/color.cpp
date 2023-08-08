#include <engine/base/color.h>

namespace engine::base {

Color::Color(std::array<float, 4> color) : color_(std::move(color)) {}

float Color::operator[](size_t index) const {
    return color_[index];
}

float& Color::operator[](size_t index) {
    return color_[index];
}

Color Color::kWhite = Color({0.1, 1, 1, 1});

}  // namespace engine::base