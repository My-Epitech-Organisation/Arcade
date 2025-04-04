set(COMPROOT ${CMAKE_CURRENT_SOURCE_DIR}/Components)
set(ENTROOT ${CMAKE_CURRENT_SOURCE_DIR}/Entity)
set(SYSTEMROOT ${CMAKE_CURRENT_SOURCE_DIR}/System)

add_subdirectory(Components/Position)
add_subdirectory(Components/Sprite)
add_subdirectory(Components/Velocity)
add_subdirectory(Components/Text)
set(SRC
    ${COMPROOT}/ComponentManager.cpp
    ${ENTROOT}/EntityManager.cpp
    ${SYSTEMROOT}/SystemManager.cpp
)

set(INC
    ${COMPROOT}/ComponentManager.hpp
    ${ENTROOT}/EntityManager.hpp
    ${SYSTEMROOT}/SystemManager.hpp
)

add_library(arcade_ecs STATIC ${SRC} ${INC})
target_include_directories(arcade_ecs PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/src/Shared)
if(ENABLE_COVERAGE)
    target_compile_options(arcade_ecs PRIVATE -O0 -g --coverage -fprofile-arcs -ftest-coverage)
    target_link_libraries(arcade_ecs PRIVATE arcade_position arcade_sprite arcade_velocity arcade_text --coverage)
else()
    target_link_libraries(arcade_ecs PRIVATE arcade_position arcade_sprite arcade_velocity arcade_text)
endif()