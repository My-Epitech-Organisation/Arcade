// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** GTKFont
*/

#ifndef SRC_GRAPHICS_GTK__GTKUTILS_GTKFONT_HPP_
    #define SRC_GRAPHICS_GTK__GTKUTILS_GTKFONT_HPP_
    #include <gtk/gtk.h>
    #include <pango/pango.h>
    #include <memory>
    #include <string>
    #include <map>

namespace GTK {

class GTKFont {
 public:
    GTKFont();
    ~GTKFont() = default;

    void loadFonts();
    std::shared_ptr<PangoFontDescription> loadFont(const std::string& fontPath);
    std::shared_ptr<PangoFontDescription> getFont(const std::string& fontName);
    bool hasFonts() const;

 private:
    std::map<std::string, std::shared_ptr<PangoFontDescription>> _fonts;
};

}  // namespace GTK

#endif  // SRC_GRAPHICS_GTK__GTKUTILS_GTKFONT_HPP_
