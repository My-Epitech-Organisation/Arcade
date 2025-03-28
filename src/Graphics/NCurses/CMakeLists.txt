set(SRCCURSES ${CMAKE_CURRENT_SOURCE_DIR})
set(SRCUTILS ${CMAKE_CURRENT_SOURCE_DIR}/NCursesUtils)
find_package(Curses REQUIRED)

set(SRC
    ${SRCCURSES}/NCurses.cpp
    ${SRCCURSES}/NCursesColor.cpp
    ${SRCUTILS}/NCursesWindow.cpp
    ${SRCUTILS}/NCursesColor.cpp
    ${SRCUTILS}/NCursesText.cpp
    ${SRCUTILS}/NCursesEntity.cpp
    ${SRCUTILS}/NCursesEvent.cpp
)

add_library(arcade_ncurses SHARED ${SRC})

set_target_properties(arcade_ncurses PROPERTIES PREFIX "" OUTPUT_NAME "arcade_ncurses")
target_include_directories(arcade_ncurses PUBLIC
    ${CURSES_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_CURRENT_SOURCE_DIR}
)

if(ENABLE_COVERAGE)
    target_compile_options(arcade_ncurses PUBLIC -O0 -g --coverage -fprofile-arcs -ftest-coverage)
    target_link_libraries(arcade_ncurses PUBLIC
        arcade_event
        arcade_ecs
        ${CURSES_LIBRARIES}
        --coverage
    )
else()
    target_link_libraries(arcade_ncurses PUBLIC
        arcade_event
        arcade_ecs
        ${CURSES_LIBRARIES}
    )
endif()
