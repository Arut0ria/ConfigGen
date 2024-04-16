#ifndef COLOR_MANAGER_HPP
#define COLOR_MANAGER_HPP

#include <algorithm>
#include <exception>
#include <filesystem>
#include <fstream>
#include <optional>
#include <regex>
#include <unordered_map>

#include "stb_image.h"
#include "yaml-cpp/yaml.h"

#include "config_loader.hpp"
#include "hsl.hpp"
#include "kmeans.hpp"
#include "patterns.hpp"
#include "rgb.hpp"

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

std::vector<Rgb>::const_iterator
get_darkest_color(const std::vector<Rgb> &colors);
std::vector<Rgb>::const_iterator
get_lightest_color(const std::vector<Rgb> &colors);

void update_waybar_colors(const std::vector<Rgb> &colors,
                          const std::vector<std::vector<Rgb>> &variants);

void update_hyprland_colors(const std::vector<Rgb> &colors,
                            const std::vector<std::vector<Rgb>> &variants,
                            const std::string &wallpaper);

} // namespace ColorManager

#endif