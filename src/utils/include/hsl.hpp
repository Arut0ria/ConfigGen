#ifndef HSL_HPP
#define HSL_HPP

#include <algorithm>
#include <cmath>
#include <exception>
#include <vector>

#include "rgb.hpp"

#define HSL_SIZE 3

#define LIGHT_MAX 1.0
#define LIGHT_MIN 0.0

#define SATURATION_MAX 1.0
#define SATURATION_MIN 0.0

#define HUE_MAX 360.0
#define HUE_MIN 0.0

class Hsl {
private:
  typedef double value_type;
  std::array<value_type, HSL_SIZE> values;

public:
  static constexpr double precision = 0.01;

  Hsl() : values({0, 0, 0}) {}
  Hsl(value_type h, value_type s, value_type l) : values({h, s, l}) {}
  Hsl(const Rgb &rgb) { convertRgbToHsl(rgb); }
  ~Hsl() {}

  inline value_type get_hue() const { return values[0]; }
  inline value_type get_saturation() const { return values[1]; }
  inline value_type get_lightness() const { return values[2]; }

  void set_hue(const value_type &val);
  void set_saturation(const value_type &val);
  void set_lightness(const value_type &val);

  static double euclidean_distance(const Hsl &h1, const Hsl &h2);
  void convertRgbToHsl(const Rgb &rgb);
  std::vector<Hsl> generate_variant(unsigned int var_number,
                                    double delta = 0.05);

  friend std::ostream &operator<<(std::ostream &os, const Hsl &hsl);
  friend Hsl &operator+=(Hsl &first, const Hsl &other);
  friend Hsl &operator/=(Hsl &first, const int &other);
  friend bool operator==(const Hsl &first, const Hsl &other);
  friend bool operator<(const Hsl &first, const Hsl &other);

  operator Rgb() const;
};

#endif