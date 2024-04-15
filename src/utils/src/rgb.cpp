#include "rgb.hpp"

Rgb::value_type &Rgb::operator[](size_t index) {
  if (index > RGB_SIZE)
    throw std::out_of_range("Color index out of range");

  return values[index];
}

const Rgb::value_type &Rgb::operator[](size_t index) const {
  if (index > RGB_SIZE)
    throw std::out_of_range("Color index out of range");

  return values[index];
}

Rgb Rgb::from_hex(const std::string &hex) {
  static const std::regex color_regex("#?([0-9a-fA-F]{2})"
                                      "([0-9a-fA-F]{2})"
                                      "([0-9a-fA-F]{2})");
  std::smatch match;
  if (!std::regex_match(hex, match, color_regex) || match.size() < RGB_SIZE)
    throw std::invalid_argument("Not a valid hexadecimal color.");

  return Rgb(std::stoi(match[1], nullptr, 16), std::stoi(match[2], nullptr, 16),
             std::stoi(match[3], nullptr, 16));
}

void Rgb::check_value(const value_type &val) const {
  if (val < RGB_MIN_VALUE || val > RGB_MAX_VALUE)
    throw std::invalid_argument(
        std::format("Color values must be between {} and {}.", RGB_MIN_VALUE,
                    RGB_MAX_VALUE));
}