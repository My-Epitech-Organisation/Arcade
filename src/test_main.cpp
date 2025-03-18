// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** test_main
*/
#include <dirent.h>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

static std::vector<std::pair<sf::RectangleShape, sf::Text>>
    add_lib(const sf::Font& font) {
    std::vector<std::pair<sf::RectangleShape, sf::Text>> libs;
    const std::string lib_dir = "./lib/";

    DIR* dir = opendir(lib_dir.c_str());
    if (!dir) {
        std::cerr << "Failed to open directory: " << lib_dir << std::endl;
        return libs;
    }

    struct dirent* entry;
    int position = 0;

    while ((entry = readdir(dir)) != nullptr) {
        std::string filename(entry->d_name);
        if (filename.size() >= 3 &&
            filename.substr(filename.size() - 3) == ".so") {
            sf::Text lib_text(filename, font, 24);
            lib_text.setFillColor(sf::Color::White);
            lib_text.setPosition(300.f, 200.f + position * 40.f);

            sf::FloatRect textBounds = lib_text.getLocalBounds();
            sf::RectangleShape select;
            select.setFillColor(sf::Color::Transparent);
            select.setOutlineColor(sf::Color::White);
            select.setOutlineThickness(0.0f);
            select.setSize(sf::Vector2f(
                textBounds.width + 2 * 10.0f,
                textBounds.height + 2 * 10.0f));
            select.setPosition(290.f, 195.f + position * 40.f);

            libs.push_back(std::make_pair(select, lib_text));
            position++;
        }
    }
    closedir(dir);
    return libs;
}

void launch_main_menu(void) {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Arcade");
    sf::Event event;
    sf::Font font;
    std::vector<std::pair<sf::RectangleShape, sf::Text>> libs;

    if (!font.loadFromFile("assets/fonts/arial.ttf")) {
        std::cerr << "Font not found pease add it in assets/fonts/";
        std::cerr << std::endl;
        return;
    }
    sf::Text title("Arcade Main Menu", font, 50);
    title.setFillColor(sf::Color::White);
    title.setPosition(200.f, 100.f);

    libs = add_lib(font);

    sf::Text instructions("Press ESC to exit", font, 24);
    instructions.setFillColor(sf::Color::White);
    instructions.setPosition(300.f, 500.f);

    int selectedIndex = -1;

    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonReleased) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                for (int i = 0; i < libs.size(); i++) {
                    sf::FloatRect bounds = libs[i].first.getGlobalBounds();
                    if (bounds.contains(static_cast<float>(mousePos.x),
                        static_cast<float>(mousePos.y))) {
                        selectedIndex = i;
                        std::cout << "Selected: "
                            << selectedIndex << std::endl;
                        break;
                    }
                }
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                } else if (event.key.code == sf::Keyboard::Down) {
                    if (selectedIndex < static_cast<int>(libs.size()))
                        selectedIndex++;
                    if (selectedIndex == static_cast<int>(libs.size()))
                        selectedIndex = 0;
                } else if (event.key.code == sf::Keyboard::Up) {
                    if (selectedIndex > -1)
                        selectedIndex--;
                    if (selectedIndex == -1)
                        selectedIndex = static_cast<int>(libs.size()) - 1;
                } else if (event.key.code == sf::Keyboard::Return
                    && selectedIndex >= 0) {
                    std::cout << "Selected: " << selectedIndex << std::endl;
                }
            }
        }
        for (int i = 0; i < libs.size(); i++) {
            if (i == selectedIndex)
                libs[i].first.setOutlineThickness(2.0f);
            else
                libs[i].first.setOutlineThickness(0.0f);
        }
        window.clear(sf::Color::Black);
        window.draw(title);
        for (auto &lib : libs) {
            window.draw(lib.first);
            window.draw(lib.second);
        }
        window.draw(instructions);
        window.display();
    }
}
