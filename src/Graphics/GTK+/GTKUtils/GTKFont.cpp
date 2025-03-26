// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** GTKFont implementation
*/
#include <memory>
#include <string>
#include "GTKUtils/GTKFont.hpp"
#include "GTKUtils/GTKWindow.hpp"

namespace GTK {

GTKFont::GTKFont() {}

void GTKFont::loadFonts() {
    _fonts["default"] = std::shared_ptr<PangoFontDescription>(
        pango_font_description_from_string("Sans 12"),
        pango_font_description_free);

    _fonts["monospace"] = std::shared_ptr<PangoFontDescription>(
        pango_font_description_from_string("Monospace 12"),
        pango_font_description_free);

    _fonts["title"] = std::shared_ptr<PangoFontDescription>(
        pango_font_description_from_string("Sans Bold 18"),
        pango_font_description_free);
}

std::shared_ptr<PangoFontDescription> GTKFont::loadFont(const std::string&
fontPath) {
    (void)fontPath;
    return std::shared_ptr<PangoFontDescription>(
        pango_font_description_from_string("Sans 12"),
        pango_font_description_free);
}

std::shared_ptr<PangoFontDescription> GTKFont::getFont(const std::string&
fontName) {
    if (_fonts.find(fontName) != _fonts.end()) {
        return _fonts[fontName];
    }
    return _fonts["default"];
}

bool GTKFont::hasFonts() const {
    return !_fonts.empty();
}

}  // namespace GTK
