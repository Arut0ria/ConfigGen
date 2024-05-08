#ifndef PATTERNS_HPP
#define PATTERNS_HPP

#include <string>

namespace ColorManager {
constexpr char waybar_theme_pattern[] =
    "@define-color bar-bg rgba(0, 0, 0, 0);\n\
@define-color main-bg {};\n\
@define-color main-fg {};\n\
@define-color wb-act-bg {};\n\
@define-color wb-act-fg {};\n\
@define-color wb-hvr-bg {};\n\
@define-color wb-hvr-fg {};\n";

constexpr char hyprland_color_config_pattern[] = "$wallpaper={}\n\
$foreground=rgb({})\n\
$foreground_variant=rgb({})\n\
$background=rgb({})\n\
$background_variant=rgb({})\n\
$secondary=rgb({})\n\
$secondary_variant=rgb({})\n\
";

constexpr char rofi_color_pattern[] = "* {{\n\
    main-bg:            {};\n\
    main-fg:            {};\n\
    main-br:            {};\n\
    main-ex:            {};\n\
    select-bg:          {};\n\
    select-fg:          {};\n\
}}\
";

constexpr char dunst_color_pattern[] = "\
[urgency_low]\n\
\tbackground = \"{}\"\n\
\tforeground = \"{}\"\n\
\tframe_color = \"{}\"\n\
\ticon = \"~/.config/dunst/icons/hyprdots.svg\"\n\
\ttimeout = 10\n\
\n\
[urgency_normal]\n\
\tbackground = \"{}\"\n\
\tforeground = \"{}\"\n\
\tframe_color = \"{}\"\n\
\ticon = \"~/.config/dunst/icons/hyprdots.svg\"\n\
\ttimeout = 10\n\
";

}; // namespace ColorManager

#endif