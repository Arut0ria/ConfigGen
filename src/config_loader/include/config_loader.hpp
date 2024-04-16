#ifndef CONFIG_LOADER_HPP
#define CONFIG_LOADER_HPP

#include <filesystem>
#include <optional>
#include <regex>
#include <string>
#include <unordered_map>

#include "names.hpp"

#define DEFAULT_CONFIG_PATH                                                    \
  std::filesystem::path(".config/config_gen/config.conf")

class ConfigLoader {
private:
  std::filesystem::path home_directory =
      std::filesystem::path(getenv("HOME") ? getenv("HOME") : "");

  std::unordered_map<std::string, std::string> config = {
      {CACHE_LOCATION, home_directory / ".cache/config_gen"},
      {WAYBAR_LOCATION, home_directory / ".config/waybar"},
      {WAYBAR_COLOR_FILENAME, "colors.css"},
      {HYPRLAND_COLOR_FILENAME, "colors.conf"},
      {HYPRLAND_CONFIG_LOCATION, home_directory / ".config/hypr"}};

  const std::regex comment_regex = std::regex("(?:^|[:space:]+)#(.*)$");
  std::string strip_comments(const std::string &str);

  void load_config(const std::filesystem::path &config_path);

  // Private constructor to prevent instantiation
  ConfigLoader();

public:
  ConfigLoader(const ConfigLoader &) = delete;
  ConfigLoader &operator=(const ConfigLoader &) = delete;

  // Static method  // Private constructor to prevent instantiation to get the
  // singleton instance
  static ConfigLoader &getInstance() {
    static ConfigLoader instance;
    return instance;
  }

  std::optional<std::string> get_value(const std::string &key) const {
    auto it = config.find(key);
    if (it != config.end()) {
      return it->second; // Retourne la valeur associée à la clé
    } else {
      return std::nullopt; // Retourne une valeur optionnelle vide si la clé
                           // n'est pas trouvée
    }
  }

  // Destructor
  ~ConfigLoader();
};

#endif