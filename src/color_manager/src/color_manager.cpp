#include "color_manager.hpp"

std::optional<std::vector<Hsl>>
ColorManager::load_pixels(const std::string &image_path) {
  int width, height, channels;
  unsigned char *image_data =
      stbi_load(image_path.c_str(), &width, &height, &channels, 0);

  if (image_data == nullptr) {
    std::cerr << "Couldn't load : " << image_path << std::endl;
    return {};
  }

  std::vector<Hsl> pixels;
  pixels.reserve(width * height);

  // Accéder aux pixels de l'image
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      // Calculer l'index du pixel dans le tableau
      int index = (y * width + x) * channels;

      // Accéder aux composantes de couleur du pixel
      unsigned char red = image_data[index];
      unsigned char green = image_data[index + 1];
      unsigned char blue = image_data[index + 2];

      pixels.emplace_back(Hsl(Rgb(red, green, blue)));
    }
  }

  stbi_image_free(image_data);

  return pixels;
}

bool ColorManager::is_cached(const std::filesystem::path &image_path) {
  std::filesystem::path cache_path =
      ConfigLoader::getInstance().get_value(CACHE_LOCATION).value() /
      image_path.filename().concat(extension);
  return std::filesystem::exists(cache_path);
}

void ColorManager::write_colors(const std::filesystem::path &image_path,
                                const std::vector<Hsl> &colors,
                                const std::vector<std::vector<Hsl>> &variants) {
  YAML::Emitter out;
  out << YAML::BeginMap;
  for (unsigned int i = 0; i < colors.size(); i++) {
    out << YAML::Key << std::format("color_{}", i);
    /*
      Output Variants
    */
    out << YAML::BeginMap;
    out << YAML::Key << "primary";
    out << YAML::Value << Rgb(colors[i]).to_hex();
    out << YAML::Key << "variants";
    out << YAML::BeginSeq;
    std::for_each(
        variants[i].begin(), variants[i].end(),
        [&](const Hsl &color) { out << YAML::Value << Rgb(color).to_hex(); });
    out << YAML::EndSeq;
    out << YAML::EndMap;
  }
  out << YAML::EndMap;

  std::ofstream file;
  std::filesystem::path cache_path =
      ConfigLoader::getInstance().get_value(CACHE_LOCATION).value() /
      image_path.filename().concat(extension);

  file.open(cache_path, std::ios::out | std::ios::trunc);
  file << out.c_str();
  file.close();
}

void ColorManager::update_colors(const std::string &image_path) {
  /*
      Perform Kmeans
  */
  auto pixels =
      std::make_shared<std::vector<Hsl>>(load_pixels(image_path).value());
  unsigned int k = 3;
  Kmeans<Hsl> kmeans(pixels, k, Hsl::euclidean_distance);
  auto centroids = kmeans.get_result();

  /*
    Generate Variants
  */
  std::vector<std::vector<Hsl>> variants(k, std::vector<Hsl>());
  for (unsigned int i = 0; i < centroids.size(); i++) {
    variants[i] = centroids[i].generate_variant(2);
  }

  /*
    Write to cache
  */
  write_colors(image_path, centroids, variants);
}

std::tuple<std::vector<Rgb>, std::vector<std::vector<Rgb>>>
ColorManager::load_colors(const std::filesystem::path &image_path) {
  if (!is_cached(image_path))
    update_colors(image_path);

  std::filesystem::path cache_path =
      ConfigLoader::getInstance().get_value(CACHE_LOCATION).value() /
      image_path.filename().concat(extension);

  std::vector<Rgb> primary_colors;
  std::vector<std::vector<Rgb>> color_variants;

  YAML::Node yamlNode = YAML::LoadFile(cache_path.string());

  for (YAML::const_iterator it = yamlNode.begin(); it != yamlNode.end(); ++it) {
    YAML::Node color_map = it->second;

    const auto &primary_color = color_map["primary"].as<std::string>();
    primary_colors.push_back(Rgb::from_hex(primary_color));

    std::vector<Rgb> variants;
    const auto &variants_yaml = color_map["variants"];
    for (const auto &variant : variants_yaml) {
      variants.push_back(Rgb::from_hex(variant.as<std::string>()));
    }
    color_variants.push_back(variants);
  }

  return std::make_tuple(primary_colors, color_variants);
}

std::vector<Rgb>::const_iterator ColorManager::get_lightest_color(const std::vector<Rgb> &colors) {
  return std::max_element(
      colors.begin(), colors.end(), [](const Rgb &rgb, const Rgb &other) {
        return rgb.get_r() + rgb.get_g() + rgb.get_b() <
               other.get_r() + other.get_g() + other.get_b();
      });
}

std::vector<Rgb>::const_iterator ColorManager::get_darkest_color(const std::vector<Rgb> &colors) {
  return std::min_element(
      colors.begin(), colors.end(), [](const Rgb &rgb, const Rgb &other) {
        return rgb.get_r() + rgb.get_g() + rgb.get_b() <
               other.get_r() + other.get_g() + other.get_b();
      });
}

void ColorManager::update_waybar_colors(
    const std::vector<Rgb> &colors,
    const std::vector<std::vector<Rgb>> &variants) {
  auto waybar_theme_file =
      std::filesystem::path(
          ConfigLoader::getInstance().get_value(WAYBAR_LOCATION).value()) /
      ConfigLoader::getInstance().get_value(WAYBAR_COLOR_FILENAME).value();

  auto lightest_color = get_lightest_color(colors);
  auto darkest_color = get_darkest_color(colors);

  std::ofstream file(waybar_theme_file, std::ios::out | std::ios::trunc);
  if (!file.is_open()) {
    throw std::ofstream::failure("Couldn't write file : " + waybar_theme_file.string());
  }

  auto darkest_index = std::distance(colors.begin(), darkest_color);
  file << std::format(
      waybar_theme_pattern, darkest_color->to_hex(),
      variants[darkest_index][0].to_hex(), darkest_color->to_hex(),
      variants[darkest_index][1].to_hex(), darkest_color->to_hex(),
      variants[darkest_index][1].to_hex());
  file.close();
}