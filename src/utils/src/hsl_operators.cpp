#include "hsl.hpp"
#include <iostream>

std::ostream &operator<<(std::ostream &os, const Hsl &hsl) {
  os << "HSL(" << hsl.get_hue() << ", " << hsl.get_saturation() << ", "
     << hsl.get_lightness() << ")";
  return os;
}

Hsl &operator+=(Hsl &first, const Hsl &other) {
  first.values = {first.get_hue() + other.get_hue(),
                  first.get_saturation() + other.get_saturation(),
                  first.get_lightness() + other.get_lightness()};
  return first;
}

Hsl &operator/=(Hsl &first, const int &other) {
  first.values = {first.get_hue() / other, first.get_saturation() / other,
                  first.get_lightness() / other};
  return first;
}

bool operator==(const Hsl &first, const Hsl &other) {
  return (std::abs(first.get_hue() - other.get_hue()) <= Hsl::precision) &&
         (std::abs(first.get_saturation() - other.get_saturation()) <=
          Hsl::precision) &&
         (std::abs(first.get_lightness() - other.get_lightness()) <=
          Hsl::precision);
}

bool operator<(const Hsl &first, const Hsl &other) {
  Hsl::value_type distance_squared_this =
      first.get_hue() * other.get_hue() +
      first.get_saturation() * other.get_saturation() +
      first.get_lightness() * other.get_lightness();
  Hsl::value_type distance_squared_other =
      other.get_hue() * other.get_hue() +
      other.get_saturation() * other.get_saturation() +
      other.get_lightness() * other.get_lightness();

  return distance_squared_this < distance_squared_other;
}