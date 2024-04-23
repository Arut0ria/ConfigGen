#include <chrono>
#include <format>
#include <iostream>
#include <tuple>
#include <vector>

#include "color_manager.hpp"
#include "timer.hpp"

// #define TEST_IMAGE "/home/theo/.config/swww/Wallpapers/52481425_p0.png"

int main(int argc, char *argv[]) {
  Timer timer("Total time : ", true);

  if (argc < 2)
    return EXIT_FAILURE;

  // TIMEIT(ColorManager::update_colors(argv[1]));
  auto [colors, variants] = TIMEIT(ColorManager::load_colors(argv[1]));
  TIMEIT(ColorManager::update_waybar_colors(colors, variants));
  TIMEIT(ColorManager::update_hyprland_colors(colors, variants, argv[1]));
  TIMEIT(ColorManager::update_rofi_colors(colors, variants));

  return EXIT_SUCCESS;
}