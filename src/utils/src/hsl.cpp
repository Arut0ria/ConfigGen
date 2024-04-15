#include "hsl.hpp"

double Hsl::euclidean_distance(const Hsl &h1, const Hsl &h2) {
  return std::sqrt(std::pow(h1.get_hue() - h2.get_hue(), 2) +
                   std::pow(h1.get_saturation() - h2.get_saturation(), 2) +
                   std::pow(h1.get_lightness() - h2.get_lightness(), 2));
}

void Hsl::set_hue(const value_type &val) {
  if (val > HUE_MAX || val < HUE_MIN)
    throw std::invalid_argument(
        std::format("Hue value must be between {} and {}", HUE_MIN, HUE_MAX));
  values[0] = val;
}

void Hsl::set_saturation(const value_type &val) {
  if (val > SATURATION_MAX || val < SATURATION_MIN) {
    throw std::invalid_argument(
        std::format("Saturation value must be between {} and {}",
                    SATURATION_MIN, SATURATION_MAX));
  }
  values[1] = val;
}

void Hsl::set_lightness(const value_type &val) {
  if (val > LIGHT_MAX || val < LIGHT_MIN)
    throw std::invalid_argument(std::format(
        "Light value must be between {} and {}", LIGHT_MIN, LIGHT_MAX));
  values[2] = val;
}

void Hsl::convertRgbToHsl(const Rgb &rgb) {
  // Convertir les valeurs R, G et B de [0, 255] à [0, 1]
  double red = rgb.get_r() / 255.0;
  double green = rgb.get_g() / 255.0;
  double blue = rgb.get_b() / 255.0;

  double cmax = std::max({red, green, blue});
  double cmin = std::min({red, green, blue});
  double delta = cmax - cmin;

  // Calcul de la luminosité (lightness)
  set_lightness((cmax + cmin) / 2.0);

  // Si delta est nul, la couleur est un gris, donc la saturation est de 0
  if (delta == 0.0) {
    set_hue(HUE_MIN);
    set_saturation(SATURATION_MIN);
  } else {
    // Calcul de la saturation
    double sat = delta / (1.0 - std::abs(2.0 * get_lightness() - 1.0));
    if(sat > SATURATION_MAX) sat = SATURATION_MAX;
    else if (sat < SATURATION_MIN) sat = SATURATION_MIN;
    set_saturation(sat);

    // Calcul de la teinte (hue)
    double h = HUE_MIN;
    if (cmax == red) {
      h = 60 * std::fmod(((green - blue) / delta), 6);
    } else if (cmax == green) {
      h = 60 * (((blue - red) / delta) + 2);
    } else if (cmax == blue) {
      h = 60 * (((red - green) / delta) + 4);
    }

    // Assurer que la teinte reste dans la plage [0, 360]
    if (h < HUE_MIN) {
      h += HUE_MAX;
    }

    set_hue(h);
  }
}

std::vector<Hsl> Hsl::generate_variant(unsigned int var_number, double delta) {
  // If light color -> generate darker shades
  if (get_lightness() > LIGHT_MAX / 2.0)
    delta = -delta;

  std::vector<Hsl> variants;
  variants.reserve(var_number);

  value_type new_light = get_lightness();
  value_type new_sat = get_saturation();

  for (unsigned int i = 0; i < var_number; i++) {
    new_light += delta;
    new_sat -= delta;

    if (new_light < LIGHT_MIN)
      new_light = LIGHT_MIN;
    else if (new_light > LIGHT_MAX)
      new_light = LIGHT_MAX;

    if (new_sat < SATURATION_MIN)
      new_sat = SATURATION_MIN;
    else if (new_sat > SATURATION_MAX)
      new_sat = SATURATION_MAX;

    variants.emplace_back(Hsl(get_hue(), new_sat, new_light));
  }

  return variants;
}

Hsl::operator Rgb() const {
  // Calcul des composantes RGB à partir des composantes HSL

  value_type hue = get_hue();
  value_type saturation = get_saturation();
  value_type lightness = get_lightness();

  double chroma = (1 - std::abs(2 * lightness - 1)) * saturation;
  double hue_prime = hue / 60.0;
  double x = chroma * (1 - std::abs(std::fmod(hue_prime, 2) - 1));

  double red = 0, green = 0, blue = 0;

  if (hue_prime >= 0 && hue_prime < 1) {
    red = chroma;
    green = x;
  } else if (hue_prime >= 1 && hue_prime < 2) {
    red = x;
    green = chroma;
  } else if (hue_prime >= 2 && hue_prime < 3) {
    green = chroma;
    blue = x;
  } else if (hue_prime >= 3 && hue_prime < 4) {
    green = x;
    blue = chroma;
  } else if (hue_prime >= 4 && hue_prime < 5) {
    red = x;
    blue = chroma;
  } else if (hue_prime >= 5 && hue_prime < 6) {
    red = chroma;
    blue = x;
  }

  double m = lightness - chroma / 2;
  red += m;
  green += m;
  blue += m;

  // Convertir les valeurs RGB de [0, 1] à [0, 255]
  unsigned int r = static_cast<unsigned int>(red * RGB_MAX_VALUE);
  unsigned int g = static_cast<unsigned int>(green * RGB_MAX_VALUE);
  unsigned int b = static_cast<unsigned int>(blue * RGB_MAX_VALUE);

  return Rgb(r, g, b);
}