set(NAME "arcade_snake")

set(SRC
    Snake.cpp
    Snake.hpp
    SnakeFactory.cpp
    SnakeFactory.hpp
    Components/FoodComponent.cpp
    Components/GridComponent.cpp
    Components/SnakeHeadComponent.cpp
    System/EventSubSystem.cpp
    System/GameLogic.cpp
    System/UISystem.cpp
)

add_library(${NAME} SHARED ${SRC})
target_include_directories(${NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src)
set_target_properties(${NAME} PROPERTIES PREFIX "")

if(ENABLE_COVERAGE)
    target_compile_options(${NAME} PRIVATE -O0 -g --coverage -fprofile-arcs -ftest-coverage)
    target_link_libraries(${NAME} PRIVATE arcade_ecs arcade_drawable arcade_parser --coverage)
else()
    target_link_libraries(${NAME} PRIVATE arcade_ecs arcade_drawable arcade_parser)
endif()