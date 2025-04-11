// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** B-OOP-400 Arcade
** File description:
** Snake game module implementation
*/

#include <memory>
#include <random>
#include "Games/Snake/Snake.hpp"
#include "Games/Snake/System/RenderSystem.hpp"
#include "Games/Snake/System/GameLogic.hpp"
#include "Games/Snake/Components/Cell.hpp"
#include "Games/Snake/Components/Snake.hpp"
#include "Games/Snake/Components/Food.hpp"
#include "ECS/Components/Position.hpp"
#include "ECS/Components/Sprite.hpp"
#include "ECS/Components/Text.hpp"

namespace Arcade {

SnakeGame::~SnakeGame() {}

void SnakeGame::init(std::shared_ptr<IEventManager> eventManager,
    std::shared_ptr<IComponentManager> componentManager,
    std::shared_ptr<IEntityManager> entityManager) {
    _eventManager = eventManager;
    _componentManager = componentManager;
    _entityManager = entityManager;

    // Register components
    _componentManager->registerComponent<Board>();
    _componentManager->registerComponent<Cell>();
    _componentManager->registerComponent<Snake>();
    _componentManager->registerComponent<Food>();
    _componentManager->registerComponent<GameStats>();

    // Create systems
    _eventSystem = std::make_shared<EventSubSystem>(_eventManager, _componentManager,
                                                   _entityManager);
    auto renderSystem = std::make_shared<RenderSystem>(_componentManager,
                                                     _entityManager);
    auto uiSystem = std::make_shared<UISystem>(_componentManager, _entityManager);
    auto gameLogic = std::make_shared<GameLogic>(_componentManager, _entityManager);
    auto movementSystem = std::make_shared<MovementSystem>(_componentManager,
                                                         _entityManager, this);

    _systems.push_back(_eventSystem);
    _systems.push_back(renderSystem);
    _systems.push_back(uiSystem);
    _systems.push_back(gameLogic);
    _systems.push_back(movementSystem);

    createBoard();
    createSnake();
    spawnFood();
}

void SnakeGame::update() {
    for (auto& system : _systems) {
        system->update();
    }

    // Check if game over
    auto entities = _entityManager->getEntitiesWithComponent<Snake>();
    if (entities.empty()) {
        _gameOver = true;
        return;
    }

    // Update score from stats
    auto statsEntities = _entityManager->getEntitiesWithComponent<GameStats>();
    if (!statsEntities.empty()) {
        Entity statsEntity = statsEntities[0];
        auto stats = _componentManager->getComponent<GameStats>(statsEntity);
        _score = stats->score;
    }
}

bool SnakeGame::isGameOver() const {
    return _gameOver;
}

bool SnakeGame::hasWon() const {
    return _gameWon;
}

void SnakeGame::stop() {
    _systems.clear();
}

int SnakeGame::getScore() const {
    return _score;
}

std::string SnakeGame::getSpecialCompSprite(size_t id) const {
    if (id == Snake::HEAD_UP) return "head_up";
    if (id == Snake::HEAD_DOWN) return "head_down";
    if (id == Snake::HEAD_LEFT) return "head_left";
    if (id == Snake::HEAD_RIGHT) return "head_right";
    if (id == Snake::BODY_VERTICAL) return "body_vertical";
    if (id == Snake::BODY_HORIZONTAL) return "body_horizontal";
    if (id == Snake::BODY_TOPLEFT) return "body_topleft";
    if (id == Snake::BODY_TOPRIGHT) return "body_topright";
    if (id == Snake::BODY_BOTTOMLEFT) return "body_bottomleft";
    if (id == Snake::BODY_BOTTOMRIGHT) return "body_bottomright";
    if (id == Snake::TAIL_UP) return "tail_up";
    if (id == Snake::TAIL_DOWN) return "tail_down";
    if (id == Snake::TAIL_LEFT) return "tail_left";
    if (id == Snake::TAIL_RIGHT) return "tail_right";
    if (id == Food::APPLE) return "apple";
    return "background";
}

void SnakeGame::createBoard() {
    // Create board entity
    Entity boardEntity = _entityManager->createEntity();
    Board board(20, 20);  // 20x20 grid
    _componentManager->addComponent(boardEntity, board);

    // Create game stats entity
    Entity statsEntity = _entityManager->createEntity();
    GameStats stats;
    stats.score = 0;
    stats.time = 0;
    _componentManager->addComponent(statsEntity, stats);

    // Create cells
    for (int i = 0; i < board.width; i++) {
        for (int j = 0; j < board.height; j++) {
            Entity cellEntity = _entityManager->createEntity();
            Cell cell;
            cell.type = Cell::EMPTY;
            cell.x = i;
            cell.y = j;

            Position pos;
            pos.x = i * 32;  // Cell size is 32x32 pixels
            pos.y = j * 32;

            Sprite sprite;
            sprite.texturePath = "assets/snake/background.png";
            sprite.width = 32;
            sprite.height = 32;

            _componentManager->addComponent(cellEntity, cell);
            _componentManager->addComponent(cellEntity, pos);
            _componentManager->addComponent(cellEntity, sprite);
        }
    }
}

void SnakeGame::createSnake() {
    // Create snake entity
    Entity snakeEntity = _entityManager->createEntity();

    // Starting position in the middle of the board
    int startX = 10;
    int startY = 10;

    // Initial snake with 3 segments
    Snake snake;
    snake.segments = {
        {startX, startY},       // Head
        {startX - 1, startY},   // Body
        {startX - 2, startY}    // Tail
    };
    snake.direction = Snake::RIGHT;
    snake.growNextUpdate = false;

    _componentManager->addComponent(snakeEntity, snake);

    // Create visual representation for each snake segment
    updateSnakeVisuals();
}

void SnakeGame::spawnFood() {
    std::random_device rd;
    std::mt19937 gen(rd());

    auto boardEntities = _entityManager->getEntitiesWithComponent<Board>();
    if (boardEntities.empty()) return;

    Entity boardEntity = boardEntities[0];
    auto board = _componentManager->getComponent<Board>(boardEntity);

    // Get snake to avoid spawning food on it
    auto snakeEntities = _entityManager->getEntitiesWithComponent<Snake>();
    if (snakeEntities.empty()) return;

    Entity snakeEntity = snakeEntities[0];
    auto snake = _componentManager->getComponent<Snake>(snakeEntity);

    // Find empty cell for food
    std::uniform_int_distribution<> disX(0, board->width - 1);
    std::uniform_int_distribution<> disY(0, board->height - 1);

    int x, y;
    bool validPosition;

    do {
        x = disX(gen);
        y = disY(gen);

        validPosition = true;
        for (const auto& segment : snake->segments) {
            if (segment.first == x && segment.second == y) {
                validPosition = false;
                break;
            }
        }
    } while (!validPosition);

    // Create food entity
    Entity foodEntity = _entityManager->createEntity();

    Food food;
    food.x = x;
    food.y = y;
    food.type = Food::APPLE;

    Position pos;
    pos.x = x * 32;  // Cell size is 32x32 pixels
    pos.y = y * 32;

    Sprite sprite;
    sprite.texturePath = "assets/snake/apple.png";
    sprite.width = 32;
    sprite.height = 32;
    sprite.specialCompId = Food::APPLE;

    _componentManager->addComponent(foodEntity, food);
    _componentManager->addComponent(foodEntity, pos);
    _componentManager->addComponent(foodEntity, sprite);
}

// Helper method to update snake visuals based on segments
void SnakeGame::updateSnakeVisuals() {
    auto snakeEntities = _entityManager->getEntitiesWithComponent<Snake>();
    if (snakeEntities.empty()) return;

    Entity snakeEntity = snakeEntities[0];
    auto snake = _componentManager->getComponent<Snake>(snakeEntity);

    // First, remove old visual representation
    auto cellEntities = _entityManager->getEntitiesWithComponent<Cell>();
    for (auto entity : cellEntities) {
        auto cell = _componentManager->getComponent<Cell>(entity);
        if (cell->type == Cell::SNAKE_HEAD || cell->type == Cell::SNAKE_BODY || cell->type == Cell::SNAKE_TAIL) {
            cell->type = Cell::EMPTY;
            auto sprite = _componentManager->getComponent<Sprite>(entity);
            if (sprite) {
                sprite->texturePath = "assets/snake/background.png";
                sprite->specialCompId = 0;
            }
        }
    }

    // Then create new visual representation
    for (size_t i = 0; i < snake->segments.size(); i++) {
        const auto& segment = snake->segments[i];
        int x = segment.first;
        int y = segment.second;

        // Find cell entity at this position
        for (auto entity : cellEntities) {
            auto cell = _componentManager->getComponent<Cell>(entity);
            if (cell->x == x && cell->y == y) {
                auto sprite = _componentManager->getComponent<Sprite>(entity);

                if (i == 0) {  // Head
                    cell->type = Cell::SNAKE_HEAD;
                    if (snake->direction == Snake::UP) {
                        sprite->specialCompId = Snake::HEAD_UP;
                    } else if (snake->direction == Snake::DOWN) {
                        sprite->specialCompId = Snake::HEAD_DOWN;
                    } else if (snake->direction == Snake::LEFT) {
                        sprite->specialCompId = Snake::HEAD_LEFT;
                    } else {
                        sprite->specialCompId = Snake::HEAD_RIGHT;
                    }
                } else if (i == snake->segments.size() - 1) {  // Tail
                    cell->type = Cell::SNAKE_TAIL;
                    // Determine tail orientation based on preceding segment
                    const auto& prevSegment = snake->segments[i - 1];
                    if (prevSegment.first == x && prevSegment.second < y) {
                        sprite->specialCompId = Snake::TAIL_UP;
                    } else if (prevSegment.first == x && prevSegment.second > y) {
                        sprite->specialCompId = Snake::TAIL_DOWN;
                    } else if (prevSegment.first < x && prevSegment.second == y) {
                        sprite->specialCompId = Snake::TAIL_LEFT;
                    } else {
                        sprite->specialCompId = Snake::TAIL_RIGHT;
                    }
                } else {  // Body
                    cell->type = Cell::SNAKE_BODY;
                    // Determine body segment orientation
                    const auto& prevSegment = snake->segments[i - 1];
                    const auto& nextSegment = snake->segments[i + 1];

                    // Straight segments
                    if (prevSegment.first == nextSegment.first) {
                        sprite->specialCompId = Snake::BODY_VERTICAL;
                    } else if (prevSegment.second == nextSegment.second) {
                        sprite->specialCompId = Snake::BODY_HORIZONTAL;
                    } else {
                        // Corner segments
                        if ((prevSegment.second < y && nextSegment.first > x) || 
                            (prevSegment.first > x && nextSegment.second < y)) {
                            sprite->specialCompId = Snake::BODY_TOPLEFT;
                        } else if ((prevSegment.second < y && nextSegment.first < x) || 
                                   (prevSegment.first < x && nextSegment.second < y)) {
                            sprite->specialCompId = Snake::BODY_TOPRIGHT;
                        } else if ((prevSegment.second > y && nextSegment.first > x) || 
                                   (prevSegment.first > x && nextSegment.second > y)) {
                            sprite->specialCompId = Snake::BODY_BOTTOMLEFT;
                        } else {
                            sprite->specialCompId = Snake::BODY_BOTTOMRIGHT;
                        }
                    }
                }

                break;
            }
        }
    }
}

}  // namespace Arcade
