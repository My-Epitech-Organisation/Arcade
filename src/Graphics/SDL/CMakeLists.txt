set(SRCSDL ${CMAKE_CURRENT_SOURCE_DIR})
find_package(SDL2 REQUIRED)

set(SRC
    ${SRCSDL}/SDL.cpp
)
add_library(arcade_sdl2 SHARED ${SRC})

set_target_properties(arcade_sdl2 PROPERTIES PREFIX "" OUTPUT_NAME "arcade_sdl2")
target_include_directories(arcade_sdl2 PUBLIC ${SDL2_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/..)
if(ENABLE_COVERAGE)
    target_compile_options(arcade_sdl2 PUBLIC -O0 -g --coverage -fprofile-arcs -ftest-coverage)
    target_link_libraries(arcade_sdl2 PUBLIC ${SDL2_LIBRARIES} --coverage)
else ()
    target_link_libraries(arcade_sdl2 PUBLIC ${SDL2_LIBRARIES})
endif()
