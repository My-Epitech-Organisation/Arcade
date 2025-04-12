set(NAME "arcade_minesweeper")

set(SRC
    Minesweeper.cpp
    MinesweeperFactory.cpp
    Components/Board.cpp
    Components/GameStats.cpp
    System/EventSubSystem.cpp
    System/GameLogic.cpp
    System/RenderSystem.cpp
    System/UISystem.cpp
)

add_library(${NAME} SHARED ${SRC})
target_include_directories(${NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src)
set_target_properties(${NAME} PROPERTIES PREFIX "")

if(ENABLE_COVERAGE)
    target_compile_options(${NAME} PRIVATE -O0 -g --coverage -fprofile-arcs -ftest-coverage)
    target_link_libraries(${NAME} PRIVATE arcade_ecs arcade_drawable arcade_score arcade_parser --coverage)
else()
    target_link_libraries(${NAME} PRIVATE arcade_ecs arcade_drawable arcade_score arcade_parser)
endif()