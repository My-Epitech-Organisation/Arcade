cmake_minimum_required(VERSION 3.22)
project(Arcade)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(ENABLE_TESTS "Enable tests" OFF)
option(ENABLE_DOCS "Enable documentation" OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

add_subdirectory(src)

if (ENABLE_DOCS)
    add_subdirectory(docs)
endif()

if (ENABLE_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()