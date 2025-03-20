set(SRCCURSES ${CMAKE_CURRENT_SOURCE_DIR})
find_package(Curses REQUIRED)

set(SRC
    ${SRCCURSES}/nCurses.cpp
)

add_library(arcade_ncurses SHARED ${SRC})

set_target_properties(arcade_ncurses PROPERTIES PREFIX "" OUTPUT_NAME "arcade_ncurses")
target_include_directories(arcade_ncurses PUBLIC ${NCURSES_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/..)

if(ENABLE_COVERAGE)
    target_compile_options(arcade_ncurses PUBLIC -O0 -g --coverage -fprofile-arcs -ftest-coverage)
    target_link_libraries(arcade_ncurses PUBLIC arcade_shared --coverage)
else ()
    target_link_libraries(arcade_ncurses PUBLIC arcade_shared ${NCURSES_LIBRARIES} --coverage)
endif()
