cmake_minimum_required(VERSION 3.22)
project(Arcade)
set(CMAKE_CXX_STANDARD 20)

option(ENABLE_TESTS "Enable tests" OFF)
option(ENABLE_DOCS "Enable documentation" OFF)
add_subdirectory(src)

if (ENABLE_DOCS)
    add_subdirectory(docs)
endif()

if (ENABLE_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()