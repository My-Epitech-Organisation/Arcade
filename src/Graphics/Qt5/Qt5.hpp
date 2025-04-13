// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Qt5 class
*/

#ifndef SRC_GRAPHICS_QT5_QT5_HPP_
    #define SRC_GRAPHICS_QT5_QT5_HPP_
    #include <QApplication>
    #include <QMainWindow>
    #include <QPainter>
    #include <QPixmap>
    #include <QKeyEvent>
    #include <QMouseEvent>
    #include <QFontDatabase>
    #include <QCursor>
    #include <QPoint>
    #include <memory>
    #include <utility>
    #include <string>
    #include <unordered_map>
    #include <thread>
    #include <mutex>
    #include <condition_variable>
    #include <queue>
    #include <functional>
    #include <atomic>
    #include "Interface/Display/IDisplayModule.hpp"
    #include "Interface/Display/IDrawableComponent.hpp"
    #include "Models/ColorType.hpp"

class ArcadeWidget : public QWidget {
    Q_OBJECT

 private:
    QPixmap* _buffer;

 public:
    explicit ArcadeWidget(QWidget* parent = nullptr)
        : QWidget(parent), _buffer(nullptr) {
        setMouseTracking(true);
    }

    void setBuffer(QPixmap* buffer) {
        _buffer = buffer;
    }This pull request includes various changes to the `pacman.json` configuration, updates to the `EventManager` class, and enhancements to the `GameLoop` class. The most important changes are listed below:

### Configuration Updates:
* [`config/pacman.json`](diffhunk://#diff-e070e577ed3db784ce1b615174577720889ec093fc8424e4c0593732feaf6130L14-L93): Refactored the sprite configuration by replacing arrays with individual keys for each direction's frames. This change simplifies the structure and improves readability.

### Event Management Enhancements:
* `src/Core/EventManager/EventManager.cpp` and `src/Core/EventManager/EventManager.hpp`: Added new methods such as `unsubscribe`, `resetKeys`, `setKeyState`, `unsubscribeAll(const IEvent&)`, and `isEventSubscribed`. These methods improve the flexibility and robustness of event handling. [[1]](diffhunk://#diff-e2f7ad02bba9723d2f1bbde4bb4cdd696978bdcbc589da0f92d1a8966a0f53f6R174-R196) [[2]](diffhunk://#diff-e2f7ad02bba9723d2f1bbde4bb4cdd696978bdcbc589da0f92d1a8966a0f53f6R238-R251) [[3]](diffhunk://#diff-8ba0c4695d41aa367519e62a2a21991590bec7cd294d249f251279c676005ebbR129-R149)

### Game Loop Improvements:
* [`src/Core/GameLoop/GameLoop.cpp`](diffhunk://#diff-0aa13d329ef2b43294aadb69c518a792a5a3d283b548a95bf8cf1e3194f14fc6L95-L130): Refactored the `run` method to improve error handling and performance tracking, and added new member variables to track frame time and performance. The `handleState` method now marks the component cache for refresh when the state changes. [[1]](diffhunk://#diff-0aa13d329ef2b43294aadb69c518a792a5a3d283b548a95bf8cf1e3194f14fc6L95-L130) [[2]](diffhunk://#diff-0aa13d329ef2b43294aadb69c518a792a5a3d283b548a95bf8cf1e3194f14fc6L139-R148) [[3]](diffhunk://#diff-0aa13d329ef2b43294aadb69c518a792a5a3d283b548a95bf8cf1e3194f14fc6L44-R55)

### Codebase Simplification:
* [`docs/HowToImplementThings.md`](diffhunk://#diff-c287a10ad9ceb750bad7f11dc1e69607e4cbb9dc63f9f291681fe14f66e73358L207-R207): Updated the `init` method signature in the graphics library module example to use `Arcade::IWindowModule` instead of width and height parameters.
* [`src/Core/CMakeLists.txt`](diffhunk://#diff-055c2f01762e09108db37267dbdfc4197784aa1377f5c5e038a389728ab6a2edL32-R37): Added `arcade_score` to the list of linked libraries to ensure proper linkage and coverage.

 protected:
    void paintEvent(QPaintEvent*) override {
        if (_buffer) {
            QPainter painter(this);
            painter.drawPixmap(0, 0, *_buffer);
        }
    }

    void keyPressEvent(QKeyEvent* event) override {
        event->accept();
        QWidget::keyPressEvent(event);
    }

    void keyReleaseEvent(QKeyEvent* event) override {
        event->accept();
        QWidget::keyReleaseEvent(event);
    }

    void mousePressEvent(QMouseEvent* event) override {
        event->accept();
        QWidget::mousePressEvent(event);
    }

    void mouseReleaseEvent(QMouseEvent* event) override {
        event->accept();
        QWidget::mouseReleaseEvent(event);
    }

    void mouseMoveEvent(QMouseEvent* event) override {
        event->accept();
        QWidget::mouseMoveEvent(event);
    }
};

class ArcadeEventFilter : public QObject {
    Q_OBJECT

 private:This pull request includes various changes to the `pacman.json` configuration, updates to the `EventManager` class, and enhancements to the `GameLoop` class. The most important changes are listed below:

### Configuration Updates:
* [`config/pacman.json`](diffhunk://#diff-e070e577ed3db784ce1b615174577720889ec093fc8424e4c0593732feaf6130L14-L93): Refactored the sprite configuration by replacing arrays with individual keys for each direction's frames. This change simplifies the structure and improves readability.

### Event Management Enhancements:
* `src/Core/EventManager/EventManager.cpp` and `src/Core/EventManager/EventManager.hpp`: Added new methods such as `unsubscribe`, `resetKeys`, `setKeyState`, `unsubscribeAll(const IEvent&)`, and `isEventSubscribed`. These methods improve the flexibility and robustness of event handling. [[1]](diffhunk://#diff-e2f7ad02bba9723d2f1bbde4bb4cdd696978bdcbc589da0f92d1a8966a0f53f6R174-R196) [[2]](diffhunk://#diff-e2f7ad02bba9723d2f1bbde4bb4cdd696978bdcbc589da0f92d1a8966a0f53f6R238-R251) [[3]](diffhunk://#diff-8ba0c4695d41aa367519e62a2a21991590bec7cd294d249f251279c676005ebbR129-R149)

### Game Loop Improvements:
* [`src/Core/GameLoop/GameLoop.cpp`](diffhunk://#diff-0aa13d329ef2b43294aadb69c518a792a5a3d283b548a95bf8cf1e3194f14fc6L95-L130): Refactored the `run` method to improve error handling and performance tracking, and added new member variables to track frame time and performance. The `handleState` method now marks the component cache for refresh when the state changes. [[1]](diffhunk://#diff-0aa13d329ef2b43294aadb69c518a792a5a3d283b548a95bf8cf1e3194f14fc6L95-L130) [[2]](diffhunk://#diff-0aa13d329ef2b43294aadb69c518a792a5a3d283b548a95bf8cf1e3194f14fc6L139-R148) [[3]](diffhunk://#diff-0aa13d329ef2b43294aadb69c518a792a5a3d283b548a95bf8cf1e3194f14fc6L44-R55)

### Codebase Simplification:
* [`docs/HowToImplementThings.md`](diffhunk://#diff-c287a10ad9ceb750bad7f11dc1e69607e4cbb9dc63f9f291681fe14f66e73358L207-R207): Updated the `init` method signature in the graphics library module example to use `Arcade::IWindowModule` instead of width and height parameters.
* [`src/Core/CMakeLists.txt`](diffhunk://#diff-055c2f01762e09108db37267dbdfc4197784aa1377f5c5e038a389728ab6a2edL32-R37): Added `arcade_score` to the list of linked libraries to ensure proper linkage and coverage.
    std::reference_wrapper<std::unordered_map<int, bool>> _keyStates;
    std::reference_wrapper<std::unordered_map<int, bool>> _mouseButtonStates;
    std::reference_wrapper<std::pair<size_t, size_t>> _mousePosition;

 public:
    ArcadeEventFilter(std::reference_wrapper<std::unordered_map<int, bool>>
        keyStates,
        std::reference_wrapper<std::unordered_map<int, bool>> mouseButtonStates,
        std::reference_wrapper<std::pair<size_t, size_t>> mousePosition)
        : _keyStates(keyStates),
            _mouseButtonStates(mouseButtonStates),
            _mousePosition(mousePosition) {}

 protected:
    bool eventFilter(QObject* obj, QEvent* event) override {
        switch (event->type()) {
            case QEvent::KeyPress: {
                QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
                _keyStates.get()[keyEvent->key()] = true;
                return false;
            }
            case QEvent::KeyRelease: {
                QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
                _keyStates.get()[keyEvent->key()] = false;
                return false;
            }
            case QEvent::MouseButtonPress: {
                QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
                _mouseButtonStates.get()[mouseEvent->button()] = true;
                _mousePosition.get().first = static_cast<size_t>
                    (mouseEvent->x());
                _mousePosition.get().second = static_cast<size_t>
                    (mouseEvent->y());
                return false;
            }
            case QEvent::MouseButtonRelease: {
                QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
                _mouseButtonStates.get()[mouseEvent->button()] = false;
                _mousePosition.get().first = static_cast<size_t>
                    (mouseEvent->x());
                _mousePosition.get().second = static_cast<size_t>
                    (mouseEvent->y());
                return false;
            }
            case QEvent::MouseMove: {
                QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
                _mousePosition.get().first = static_cast<size_t>
                    (mouseEvent->x());
                _mousePosition.get().second = static_cast<size_t>
                    (mouseEvent->y());
                return false;
            }
            default:
                return QObject::eventFilter(obj, event);
        }
    }
};

class Qt5Module : public Arcade::IDisplayModule {
 private:
    std::string _name;
    std::unique_ptr<QApplication> _app;
    std::unique_ptr<QMainWindow> _window;
    std::unique_ptr<ArcadeWidget> _centralWidget;
    std::unique_ptr<ArcadeEventFilter> _eventFilter;
    std::unique_ptr<QPixmap> _buffer;
    std::unique_ptr<QPainter> _painter;
    int _windowWidth = 0;
    int _windowHeight = 0;
    std::unordered_map<int, bool> _keyStates;
    std::unordered_map<int, bool> _mouseButtonStates;
    std::pair<size_t, size_t> _mousePosition;

    void createWindow(int width, int height);
    QFont* loadFont(const std::string& fontPath);
    QPixmap* loadTexture(const std::string& texturePath);
    std::unordered_map<std::string, std::unique_ptr<QFont>> _fonts;
    std::unordered_map<std::string, std::unique_ptr<QPixmap>> _textures;
    std::thread _qtThread;
    std::mutex _commandMutex;
    std::condition_variable _commandCV;
    std::queue<std::function<void()>> _commandQueue;
    std::atomic<bool> _threadRunning{false};
    std::atomic<bool> _initialized{false};
    mutable std::mutex _resourceMutex;

    void threadMain();
    void executeCommand(std::function<void()> command);
    void executeCommandAndWait(std::function<void()> command);
    void drawEntity(int x, int y, char symbol);
    void drawTexture(int x, int y, const std::string &textureId);
    void drawText(const std::string &text, int x, int y,
        Arcade::Color color);

 public:
    Qt5Module() : _name("Qt5"), _running(false),
        _initialized(false), _threadRunning(false), _mousePosition({0, 0}) {}
    ~Qt5Module() override;
    void init(float width = 800.f, float height = 600.f) override;
    void stop() override;
    void clearScreen() override;
    void refreshScreen() override;
    void drawDrawable(const Arcade::DrawableComponent& drawable) override;
    void pollEvents() override;
    bool isOpen() const override;
    void drawDrawable(std::shared_ptr<Arcade::IDrawableComponent>
        drawable) override;
    const std::string& getName() const override;
    int getWidth() const override;
    int getHeight() const override;
    bool isKeyPressed(int keyCode) override;
    bool isMouseButtonPressed(int button) const override;
    std::pair<size_t, size_t> getMousePosition() const override;
    std::atomic<bool> _running{false};
};

#endif  // SRC_GRAPHICS_QT5_QT5_HPP_
