set(SRCSDL ${CMAKE_CURRENT_SOURCE_DIR})
find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2)
find_package(SDL2_TTF REQUIRED)
find_package(SDL2_IMAGE REQUIRED)

set(SRC
    ${SRCSDL}/SDL.cpp
    ${SRCSDL}/SDLUtils.cpp
)
add_library(arcade_sdl2 SHARED ${SRC})

message(STATUS "Building arcade_sdl2 library")
if (NOT SDL2_FOUND)
    message(FATAL_ERROR "SDL2 not found")
else ()
    message(STATUS "SDL2 found")
endif()
if (NOT SDL2_TTF_FOUND)
    message(FATAL_ERROR "SDL2_ttf not found")
else ()
    message(STATUS "SDL2_ttf found")
endif()
if (NOT SDL2_IMAGE_FOUND)
    message(FATAL_ERROR "SDL2_image not found")
else ()
    message(STATUS "SDL2_image found")
endif()
set_target_properties(arcade_sdl2 PROPERTIES PREFIX "" OUTPUT_NAME "arcade_sdl2")
target_include_directories(arcade_sdl2 PUBLIC ${SDL2_INCLUDE_DIRS}
    ${SDL2_TTF_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/..)
if(ENABLE_COVERAGE)
    target_compile_options(arcade_sdl2 PUBLIC -O0 -g --coverage -fprofile-arcs -ftest-coverage)
    target_link_libraries(arcade_sdl2 PUBLIC SDL2_ttf SDL2_image SDL2 arcade_event arcade_ecs --coverage)
else ()
    target_link_libraries(arcade_sdl2 PUBLIC SDL2_ttf SDL2_image SDL2 arcade_event arcade_ecs)
endif()
