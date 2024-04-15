#include "config_loader.hpp"

#include <format>
#include <fstream>
#include <iostream>

ConfigLoader::ConfigLoader() {
  load_config(home_directory / DEFAULT_CONFIG_PATH);
}

ConfigLoader::~ConfigLoader() {}

std::string ConfigLoader::strip_comments(const std::string &str) {
  return std::regex_replace(str, comment_regex, "");
}

void ConfigLoader::load_config(const std::filesystem::path &config_path) {
  std::ifstream file(config_path);
  if (!file.is_open())
    throw std::runtime_error(
        std::format("Unable to open file : {}.", config_path.string()));

  std::string line;
  while (std::getline(file, line)) {
    // Remove comments from line
    line = strip_comments(line);

    // No empty line
    if (line.empty() || line.find_first_not_of(' ') == std::string::npos)
      continue;

    // Find key value
    size_t delimiter_pos = line.find('=');
    if (delimiter_pos == std::string::npos) {
      throw std::runtime_error(
          std::format("Invalid config file format : {} in {}.", line,
                      config_path.string()));
    }

    std::string key = line.substr(0, delimiter_pos);
    std::string value = line.substr(delimiter_pos + 1);

    // Supprime les espaces de début et de fin de la clé et de la valeur
    key.erase(0, key.find_first_not_of(' '));
    key.erase(key.find_last_not_of(' ') + 1);
    value.erase(0, value.find_first_not_of(' '));
    value.erase(value.find_last_not_of(' ') + 1);

    // Stocke la paire clé-valeur dans l'unordered_map config
    config[key] = value;
  }
  file.close();
}
