find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2)
find_package(SDL2_TTF REQUIRED)
find_package(SDL2_IMAGE REQUIRED)
set(SRCSDL ${CMAKE_CURRENT_SOURCE_DIR})
set(SRCUTILS ${CMAKE_CURRENT_SOURCE_DIR}/SDLUtils)
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(SDL2_IMAGE REQUIRED SDL2_image)
pkg_check_modules(SDL2_TTF REQUIRED SDL2_ttf)

set(SRC
    ${SRCSDL}/SDL.cpp
    ${SRCUTILS}/SDLFont.cpp
    ${SRCUTILS}/SDLRenderer.cpp
    ${SRCUTILS}/SDLSurface.cpp
    ${SRCUTILS}/SDLText.cpp
    ${SRCUTILS}/SDLTexture.cpp
    ${SRCUTILS}/SDLWindow.cpp
    ${SRCUTILS}/SDLEvent.cpp
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
target_include_directories(arcade_sdl2 PUBLIC
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_IMAGE_INCLUDE_DIRS}
    ${SDL2_TTF_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_directories(arcade_sdl2 PUBLIC
    ${SDL2_LIBRARY_DIRS}
    ${SDL2_IMAGE_LIBRARY_DIRS}
    ${SDL2_TTF_LIBRARY_DIRS}
)

if(ENABLE_COVERAGE)
    target_compile_options(arcade_sdl2 PUBLIC -O0 -g --coverage -fprofile-arcs -ftest-coverage)
    target_link_libraries(arcade_sdl2 PUBLIC
        arcade_event
        arcade_ecs
        ${SDL2_LIBRARIES}
        ${SDL2_IMAGE_LIBRARIES}
        ${SDL2_TTF_LIBRARIES}
        --coverage
    )
else()
    target_link_libraries(arcade_sdl2 PUBLIC
        arcade_event
        arcade_ecs
        ${SDL2_LIBRARIES}
        ${SDL2_IMAGE_LIBRARIES}
        ${SDL2_TTF_LIBRARIES}
    )
endif()
