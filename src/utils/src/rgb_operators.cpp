#include "rgb.hpp"

std::ostream &operator<<(std::ostream &os, const Rgb &rgb) {
  os << "RGB(" << rgb[0] << ", " << rgb[1] << ", " << rgb[2] << ")";
  return os;
}