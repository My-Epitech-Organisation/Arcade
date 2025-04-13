find_package(PkgConfig REQUIRED)
pkg_check_modules(ALLEGRO5 REQUIRED allegro-5 allegro_main-5 allegro_image-5 allegro_font-5 allegro_ttf-5)

set(ALLEGRO5_UTILS_SRC
    Allegro5Utils/Allegro5Event.cpp
    Allegro5Utils/Allegro5Font.cpp
    Allegro5Utils/Allegro5Text.cpp
    Allegro5Utils/Allegro5Texture.cpp
    Allegro5Utils/Allegro5Window.cpp
)

add_library(arcade_allegro5 SHARED
    Allegro5.cpp
    ${ALLEGRO5_UTILS_SRC}
)

set_target_properties(arcade_allegro5 PROPERTIES PREFIX "" OUTPUT_NAME "arcade_allegro5")
target_include_directories(arcade_allegro5 PUBLIC
    ${ALLEGRO5_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_ROOT}/src/Shared
)
target_link_directories(arcade_allegro5 PUBLIC ${ALLEGRO5_LIBRARY_DIRS})

if(ENABLE_COVERAGE)
    target_compile_options(arcade_allegro5 PUBLIC -O0 -g --coverage -fprofile-arcs -ftest-coverage ${ALLEGRO5_CFLAGS})
    target_link_libraries(arcade_allegro5 PUBLIC
        arcade_ecs
        ${ALLEGRO5_LIBRARIES}
        --coverage
    )
else()
    target_compile_options(arcade_allegro5 PUBLIC ${ALLEGRO5_CFLAGS})
    target_link_libraries(arcade_allegro5 PUBLIC
        arcade_ecs
        ${ALLEGRO5_LIBRARIES}
    )
endif()