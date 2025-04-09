# Télécharger et ajouter GoogleTest (si non déjà installé)
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)

if(NOT TARGET arcade_core)
    add_subdirectory(${PROJECT_SOURCE_DIR}/src/Core ${CMAKE_CURRENT_BINARY_DIR})
endif()

set(LIB
    ${CMAKE_SOURCE_DIR}/lib/arcade_sfml.so
    ${CMAKE_SOURCE_DIR}/lib/arcade_sdl2.so
    ${CMAKE_SOURCE_DIR}/lib/arcade_ncurses.so
    arcade_core
)

add_executable(my_tests
    test.cpp
)
add_dependencies(my_tests arcade_core)

add_dependencies(my_tests arcade_sfml arcade_sdl2 arcade_ncurses)

target_link_libraries(my_tests PRIVATE ${LIB} gtest gtest_main)
include(GoogleTest)
gtest_discover_tests(my_tests)

option(ENABLE_COVERAGE "Enable coverage reporting" OFF)
if(ENABLE_COVERAGE)
    target_compile_options(my_tests PRIVATE -O0 -g --coverage -fprofile-arcs -ftest-coverage -fkeep-inline-functions -fno-inline)
    target_link_libraries(my_tests PRIVATE ${LIB} --coverage)
    add_custom_target(coverage
        COMMAND gcovr --root=${CMAKE_SOURCE_DIR} --object-directory=${CMAKE_BINARY_DIR}/tests/CMakeFiles/my_tests.dir/ --exclude ${CMAKE_SOURCE_DIR}/tests --exclude ${CMAKE_BINARY_DIR} --exclude ${CMAKE_SOURCE_DIR}/Documentation
        COMMENT "Displaying code coverage in terminal..."
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )
endif()

