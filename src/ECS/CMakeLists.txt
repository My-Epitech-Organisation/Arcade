set(COMPROOT ${CMAKE_CURRENT_SOURCE_DIR}/Components)
set(ENTROOT ${CMAKE_CURRENT_SOURCE_DIR}/Entity)
set(SYSTEMROOT ${CMAKE_CURRENT_SOURCE_DIR}/System)

add_subdirectory(Components/Drawable)
set(SRC
    ${COMPROOT}/ComponentManager.cpp
    ${ENTROOT}/EntityManager.cpp
    ${ENTROOT}/Entity.cpp
    ${SYSTEMROOT}/SystemManager.cpp
)

set(INC
    ${COMPROOT}/ComponentManager.hpp
    ${ENTROOT}/EntityManager.hpp
    ${ENTROOT}/Entity.hpp
    ${SYSTEMROOT}/SystemManager.hpp
)

add_library(arcade_ecs STATIC ${SRC} ${INC})
target_include_directories(arcade_ecs PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/src/Shared ${CMAKE_SOURCE_DIR}/src)
target_include_directories(arcade_ecs PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Components ${CMAKE_CURRENT_SOURCE_DIR}/Entity ${CMAKE_CURRENT_SOURCE_DIR}/System)
target_include_directories(arcade_ecs PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Components/Position ${CMAKE_CURRENT_SOURCE_DIR}/Components/Sprite ${CMAKE_CURRENT_SOURCE_DIR}/Components/Velocity ${CMAKE_CURRENT_SOURCE_DIR}/Components/Text ${CMAKE_CURRENT_SOURCE_DIR}/Components/Drawable)
target_include_directories(arcade_ecs PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/System ${CMAKE_CURRENT_SOURCE_DIR}/Shared ${CMAKE_CURRENT_SOURCE_DIR}/Components ${CMAKE_CURRENT_SOURCE_DIR}/Entity)
if(ENABLE_COVERAGE)
    target_compile_options(arcade_ecs PRIVATE -O0 -g --coverage -fprofile-arcs -ftest-coverage)
    target_link_libraries(arcade_ecs PRIVATE arcade_drawable --coverage)
else()
    target_link_libraries(arcade_ecs PRIVATE arcade_drawable)
endif()