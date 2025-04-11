set(NAME "arcade_snake")

set(SRC
    Snake.cpp
    SnakeFactory.cpp
    Components/Snake.cpp
    Components/GridComponent.cpp
    System/EventSystem.cpp
    System/MovementSystem.cpp
    System/CollisionSystem.cpp
    System/RenderSystem.cpp
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