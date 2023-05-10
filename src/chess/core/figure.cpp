#include "figure.h"

namespace chess::core {

bool Figure::operator==(const Figure& rhs) const {
    return figure_name == rhs.figure_name && is_white == rhs.is_white;
}

bool Figure::operator!=(const Figure& rhs) const {
    return !(rhs == *this);
}

}  // namespace chess::core