set(NAME "arcade_pacman")

set(SRC
    Pacman.cpp
    PacmanFactory.cpp
    Components/GridComponent.cpp
    Components/PacmanComponent.cpp
    Components/GhostComponent.cpp
    Components/FoodComponent.cpp
    System/EventSubSystem.cpp
    System/GameLogic.cpp
)

add_library(${NAME} SHARED ${SRC})
target_include_directories(${NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src)
set_target_properties(${NAME} PROPERTIES PREFIX "")

if(ENABLE_COVERAGE)
    target_compile_options(${NAME} PRIVATE -O0 -g --coverage -fprofile-arcs -ftest-coverage)
    target_link_libraries(${NAME} PRIVATE arcade_ecs --coverage)
else()
    target_link_libraries(${NAME} PRIVATE arcade_ecs)
endif()