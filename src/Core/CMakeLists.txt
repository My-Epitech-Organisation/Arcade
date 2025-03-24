set(INCCORE ${CMAKE_CURRENT_SOURCE_DIR})
set(SRCCORE ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

set(SRC
    ${SRCCORE}/core.cpp
    ${SRCCORE}/DLLoader/DLLoader.cpp
    ${SRCCORE}/GameLoop/GameLoop.cpp
)

set(INC
    ${INCCORE}/core.hpp
    ${INCCORE}/DLLoader/DLLoader.hpp
    ${INCCORE}/GameLoop/GameLoop.hpp
)

add_library(arcade_core STATIC ${SRC} ${INC})
target_include_directories(arcade_core PUBLIC ${INCCORE} ${CMAKE_CURRENT_SOURCE_DIR}/..)
add_executable(arcade main.cpp)
target_include_directories(arcade PUBLIC ${INCCORE} ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_link_libraries(arcade dl arcade_core arcade_event arcade_ecs)
if(ENABLE_COVERAGE)
    target_compile_options(arcade_core PRIVATE -O0 -g --coverage -fprofile-arcs -ftest-coverage)
    target_link_libraries(arcade_core PRIVATE arcade_event arcade_ecs --coverage)
else ()
    target_link_libraries(arcade_core PRIVATE arcade_event arcade_ecs)
endif()
