set(COMPROOT ${CMAKE_CURRENT_SOURCE_DIR}/../../ECS/Components)
set(ENTROOT ${CMAKE_CURRENT_SOURCE_DIR}/../../ECS/Entity)
set(SYSTEMROOT ${CMAKE_CURRENT_SOURCE_DIR}/../../ECS/System)
set(SHAREDROOT ${CMAKE_CURRENT_SOURCE_DIR}/../../Shared)
set(NAME "arcade_pacman")

set(SRC
    Pacman.cpp
    PacmanFactory.cpp
    Components/GridComponent.cpp
    Components/PacmanComponent.cpp
    Components/GhostComponent.cpp
    Components/FoodComponent.cpp
    Components/PacmanStats.cpp
    System/EventSubSystem.cpp
    System/GameLogic.cpp
    System/UISystem.cpp
)

set(SRC_ECS
    ${COMPROOT}/ComponentManager.cpp
    ${COMPROOT}/Position/PositionComponent.cpp
    ${COMPROOT}/Sprite/SpriteComponent.cpp
    ${COMPROOT}/Velocity/VelocityComponent.cpp
    ${COMPROOT}/Text/TextComponent.cpp
    ${COMPROOT}/Drawable/DrawableComponent.cpp
    ${ENTROOT}/EntityManager.cpp
    ${ENTROOT}/Entity.cpp
    ${SYSTEMROOT}/SystemManager.cpp
)

set(INC_ECS
    ${COMPROOT}/ComponentManager.hpp
    ${COMPROOT}/Position/Position.cpp
    ${COMPROOT}/Sprite/Sprite.cpp
    ${COMPROOT}/Velocity/Velocity.cpp
    ${COMPROOT}/Text/Text.cpp
    ${COMPROOT}/Drawable/Drawable.cpp
    ${ENTROOT}/EntityManager.hpp
    ${ENTROOT}/Entity.hpp
    ${SYSTEMROOT}/SystemManager.hpp
)

set(SRC_SHARED
    ${SHAREDROOT}/JSONParser/JSONParser.cpp
    ${SHAREDROOT}/JSONParser/Tokenizer.cpp
    ${SHAREDROOT}/ScoreProvider/ScoreProvider.cpp)

set(INC_SHARED
    ${SHAREDROOT}/JSONParser/JSONParser.hpp
    ${SHAREDROOT}/JSONParser/Tokenizer.hpp
    ${SHAREDROOT}/ScoreProvider/ScoreProvider.hpp)

add_library(${NAME} SHARED ${SRC} ${SRC_ECS} ${SRC_SHARED})
target_include_directories(${NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR}/.. ${CMAKE_CURRENT_SOURCE_DIR}/../../Shared ${CMAKE_CURRENT_SOURCE_DIR}/../../ECS ${CMAKE_CURRENT_SOURCE_DIR}/../../ECS/Components ${CMAKE_CURRENT_SOURCE_DIR}/../../ECS/System)
target_include_directories(${NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../ECS/Entity ${CMAKE_CURRENT_SOURCE_DIR}/../../ECS/Components/Position ${CMAKE_CURRENT_SOURCE_DIR}/../../ECS/Components/Sprite ${CMAKE_CURRENT_SOURCE_DIR}/../../ECS/Components/Velocity ${CMAKE_CURRENT_SOURCE_DIR}/../../ECS/Components/Text ${CMAKE_CURRENT_SOURCE_DIR}/../../ECS/Components/Drawable)
target_include_directories(${NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../ECS/System ${CMAKE_CURRENT_SOURCE_DIR}/../../Shared ${CMAKE_CURRENT_SOURCE_DIR}/../../ECS/Components ${CMAKE_CURRENT_SOURCE_DIR}/../../ECS/Entity)
target_include_directories(${NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../ECS/Components/Position ${CMAKE_CURRENT_SOURCE_DIR}/../../ECS/Components/Sprite ${CMAKE_CURRENT_SOURCE_DIR}/../../ECS/Components/Velocity ${CMAKE_CURRENT_SOURCE_DIR}/../../ECS/Components/Text ${CMAKE_CURRENT_SOURCE_DIR}/../../ECS/Components/Drawable)
target_include_directories(${NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src)
set_target_properties(${NAME} PROPERTIES PREFIX "")

if(ENABLE_COVERAGE)
    target_compile_options(${NAME} PRIVATE -O0 -g --coverage -fprofile-arcs -ftest-coverage)
    target_link_libraries(${NAME} PRIVATE --coverage)
else()
    target_link_libraries(${NAME} PRIVATE)
endif()