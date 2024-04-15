#ifndef RGB_HPP
#define RGB_HPP

#include <exception>
#include <format>
#include <iostream>
#include <regex>

#define RGB_SIZE 3
#define RGB_MAX_VALUE 255.0
#define RGB_MIN_VALUE 0.0

class Rgb {
private:
  typedef unsigned int value_type;
  std::array<value_type, RGB_SIZE> values;

public:
  Rgb() : values() {}
  Rgb(value_type r, value_type g, value_type b) : values({r, g, b}) {}

  value_type &operator[](size_t index);
  const value_type &operator[](size_t index) const;

  // Getters
  inline value_type get_r() const { return (*this)[0]; }
  inline value_type get_g() const { return (*this)[1]; }
  inline value_type get_b() const { return (*this)[2]; }

  // Setters
  inline void set_r(const value_type& red) {
    check_value(red);
    (*this)[0] = red;
  }

  inline void set_g(const value_type& green) {
    check_value(green);
    (*this)[1] = green;
  }

  inline void set_b(const value_type& blue) {
    check_value(blue);
    (*this)[2] = blue;
  }

  inline std::string to_hex() const {
    return std::format("#{:02X}{:02X}{:02X}", values[0], values[1], values[2]);
  }

  void check_value(const value_type& val) const;
  static Rgb from_hex(const std::string &hex);

  friend std::ostream &operator<<(std::ostream &os, const Rgb &rgb);
};

#endif
