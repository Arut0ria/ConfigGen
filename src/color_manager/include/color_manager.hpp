#ifndef COLOR_MANAGER_HPP
#define COLOR_MANAGER_HPP

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <optional>
#include <regex>
#include <unordered_map>
#include <exception>

#include "yaml-cpp/yaml.h"
#include "stb_image.h"

#include "patterns.hpp"
#include "config_loader.hpp"
#include "kmeans.hpp"
#include "rgb.hpp"
#include "hsl.hpp"

namespace ColorManager {
const std::string extension = ".yaml";
void update_colors(const std::string &image_path);

std::optional<std::vector<Hsl>> load_pixels(const std::string &image_path);

void write_colors(const std::filesystem::path &image_path,
                  const std::vector<Hsl> &colors,
                  const std::vector<std::vector<Hsl>> &variants);

bool is_cached(const std::filesystem::path &image_path);

std::tuple<std::vector<Rgb>, std::vector<std::vector<Rgb>>>
load_colors(const std::filesystem::path &image_path);

std::vector<Rgb>::const_iterator get_darkest_color(const std::vector<Rgb>& colors);
std::vector<Rgb>::const_iterator get_lightest_color(const std::vector<Rgb>& colors);

void update_waybar_colors(const std::vector<Rgb> &colors,
                          const std::vector<std::vector<Rgb>> &variants);

} // namespace ColorManager

#endif