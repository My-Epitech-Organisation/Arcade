find_package(Qt5 COMPONENTS Widgets REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_library(arcade_qt5 SHARED
    Qt5.cpp
    Qt5Utils.cpp
)

set_target_properties(arcade_qt5 PROPERTIES PREFIX "" OUTPUT_NAME "arcade_qt5")
target_include_directories(arcade_qt5 PUBLIC
    ${Qt5Widgets_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_SOURCE_DIR}/src/Shared
)

if(ENABLE_COVERAGE)
    target_compile_options(arcade_qt5 PUBLIC -O0 -g --coverage -fprofile-arcs -ftest-coverage)
    target_link_libraries(arcade_qt5 PUBLIC Qt5::Widgets arcade_ecs --coverage)
else ()
    target_link_libraries(arcade_qt5 PUBLIC Qt5::Widgets arcade_ecs)
endif()