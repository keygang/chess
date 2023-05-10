#pragma once

#include <functional>

namespace chess::core {

enum class FigureName {
    KING,
    QUEEN,
    ROOK,    // ladya
    BISHOP,  // slon
    KNIGHT,  // kon
    PAWN,
};

struct Figure {
    FigureName figure_name;
    bool is_white;

    bool operator==(const Figure& rhs) const;
    bool operator!=(const Figure& rhs) const;
};

}  // namespace chess::core

namespace std {

template <>
struct hash<chess::core::Figure> {
    size_t operator()(const chess::core::Figure& figure) const {
        return std::hash<size_t>()((static_cast<size_t>(figure.figure_name) << 1) +
                                   static_cast<size_t>(figure.is_white));
    }
};

}  // namespace std