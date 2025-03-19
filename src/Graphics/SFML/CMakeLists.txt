include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

add_library(arcade_sfml SHARED SFML.cpp)

set_target_properties(arcade_sfml PROPERTIES PREFIX "" OUTPUT_NAME "arcade_sfml")
target_include_directories(arcade_sfml PUBLIC ${SFML_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/..)
if(ENABLE_COVERAGE)
    target_compile_options(arcade_sfml PUBLIC -O0 -g --coverage -fprofile-arcs -ftest-coverage)
    target_link_libraries(arcade_sfml PUBLIC sfml-graphics sfml-audio sfml-window sfml-system --coverage)
else ()
    target_link_libraries(arcade_sfml PUBLIC sfml-graphics sfml-audio sfml-window sfml-system)
endif()
