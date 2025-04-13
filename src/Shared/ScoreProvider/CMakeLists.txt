set(SCOREROOT ${CMAKE_CURRENT_SOURCE_DIR}/../ScoreProvider)

set(SRC
    ${SCOREROOT}/ScoreProvider.cpp
)

set(INC
    ${SCOREROOT}/ScoreProvider.hpp
)

add_library(arcade_score STATIC ${SRC} ${INC})
target_include_directories(arcade_score PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/.. ${CMAKE_CURRENT_SOURCE_DIR}/../..)
set_target_properties(arcade_score PROPERTIES POSITION_INDEPENDENT_CODE ON)
if(ENABLE_COVERAGE)
    target_compile_options(arcade_score PRIVATE -O0 -g --coverage -fprofile-arcs -ftest-coverage)
    target_link_libraries(arcade_score PRIVATE --coverage)
else()
    target_link_libraries(arcade_score PRIVATE)
endif()