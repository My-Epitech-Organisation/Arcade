find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED gtk4)

set(GTK_UTILS_SRC
    GTKUtils/GTKWindow.cpp
    GTKUtils/GTKRenderer.cpp
    GTKUtils/GTKTexture.cpp
    GTKUtils/GTKText.cpp
    GTKUtils/GTKFont.cpp
    GTKUtils/GTKEvent.cpp
)

add_library(arcade_gtk SHARED
    GTK.cpp
    GTKUtils.cpp
    ${GTK_UTILS_SRC}
)

set_target_properties(arcade_gtk PROPERTIES PREFIX "" OUTPUT_NAME "arcade_gtk+")
target_include_directories(arcade_gtk PUBLIC
    ${GTK4_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_CURRENT_SOURCE_DIR}
)

if(ENABLE_COVERAGE)
    target_compile_options(arcade_gtk PUBLIC -O0 -g --coverage -fprofile-arcs -ftest-coverage)
    target_link_libraries(arcade_gtk PUBLIC ${GTK4_LIBRARIES} arcade_event arcade_ecs --coverage)
else()
    target_link_libraries(arcade_gtk PUBLIC ${GTK4_LIBRARIES} arcade_event arcade_ecs)
endif()

target_link_options(arcade_gtk PUBLIC ${GTK4_LDFLAGS})