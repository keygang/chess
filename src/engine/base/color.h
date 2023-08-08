#pragma once

#include <array>

namespace engine::base {

class Color {
public:
    Color() = default;
    explicit Color(std::array<float, 4> color);

    float operator[](size_t index) const;
    float& operator[](size_t index);

    static Color kWhite;
    static Color kBlack;

    // Without changing alpha
    Color get_inverse() const;

private:
    std::array<float, 4> color_ = {0};
};

}  // namespace engine::base