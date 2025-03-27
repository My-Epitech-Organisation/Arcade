/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Window
*/

#ifndef SRC_CORE_WINDOW_WINDOW_HPP_
#define SRC_CORE_WINDOW_WINDOW_HPP_

#include <memory>
#include <string>
#include "Interface/Core/IWindowModule.hpp"
#include "Interface/Display/IDisplayModule.hpp"
#include "EventManager/AEventManager.hpp"
#include "Models/KeysType.hpp"
#include "Models/ColorType.hpp"

namespace Arcade {

class Window : public IWindowModule {
 public:
    explicit Window(std::shared_ptr<IDisplayModule> displayModule,
        std::shared_ptr<AEventManager> eventManager);
    ~Window();

    void createWindow(int width, int height, const std::string &title) override;
    void resizeWindow(int width, int height) override;
    void closeWindow() override;
    bool isWindowOpen() const override;
    void clearScreen();
    void refreshScreen();
    void drawText(const std::string &text, int x, int y, Color color);
    void pollEvents();
    bool isKeyPressed(int keyCode);
    int getWidth() const;
    int getHeight() const;
    IDisplayModule* getDisplayModule();
    void setDisplayModule(std::shared_ptr<IDisplayModule> displayModule);
    std::shared_ptr<IEventManager> getEventManager() const;

 private:
    std::shared_ptr<IDisplayModule> _displayModule;
    std::shared_ptr<AEventManager> _eventManager;
    int _width;
    int _height;
    std::string _title;
    bool _isShuttingDown;
};

}  // namespace Arcade

#endif  // SRC_CORE_WINDOW_WINDOW_HPP_
