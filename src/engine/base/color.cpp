#include <engine/base/color.h>

namespace engine::base {

Color::Color(std::array<float, 4> color) : color_(std::move(color)) {}

float Color::operator[](size_t index) const {
    return color_[index];
}

float& Color::operator[](size_t index) {
    return color_[index];
}

Color Color::kWhite = Color({1, 1, 1, 1});
Color Color::kBlack = Color({0, 0, 0, 1});

Color Color::get_inverse() const {
    Color color(color_);
    for (size_t i = 0; i + 1 < color_.size(); ++i) {
        color[i] = 1.f - color[i];
    }
    return color;
}

}  // namespace engine::base