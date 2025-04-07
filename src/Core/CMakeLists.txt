set(INCCORE ${CMAKE_CURRENT_SOURCE_DIR})
set(SRCCORE ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

set(SRC
    ${SRCCORE}/DLLoader/DLLoader.tpp
    ${SRCCORE}/EventManager/EventManager.cpp
    ${SRCCORE}/GameLoop/GameLoop.cpp
    ${SRCCORE}/GameLoop/GameLoopUtils.cpp
    ${SRCCORE}/Window/Window.cpp
)

set(INC
    ${INCCORE}/DLLoader/DLLoader.hpp
    ${SRCCORE}/EventManager/EventManager.hpp
    ${INCCORE}/GameLoop/GameLoop.hpp
    ${INCCORE}/Window/Window.hpp
)

add_subdirectory(Menu)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_library(arcade_core STATIC ${SRC} ${INC})
target_include_directories(arcade_core PUBLIC ${INCCORE} ${CMAKE_CURRENT_SOURCE_DIR}/.. ${CMAKE_CURRENT_SOURCE_DIR}/../Shared)
add_executable(arcade main.cpp)
target_include_directories(arcade PUBLIC ${INCCORE} ${CMAKE_CURRENT_SOURCE_DIR}/.. ${CMAKE_CURRENT_SOURCE_DIR}/../Shared)
target_link_libraries(arcade dl arcade_core arcade_ecs)
if(ENABLE_COVERAGE)
    target_compile_options(arcade_core PRIVATE -O0 -g --coverage -fprofile-arcs -ftest-coverage)
    target_link_libraries(arcade_core PRIVATE arcade_ecs --coverage)
else ()
    target_link_libraries(arcade_core PRIVATE arcade_ecs)
endif()
