set(SRCMAIN ${PROJECT_SOURCE_DIR}/src)
set(INCMAIN ${PROJECT_SOURCE_DIR}/src/temp)

set(SRC
    ${SRCMAIN}/main.cpp
    ${SRCMAIN}/test_main.cpp
)

set(INC
    ${INCMAIN}/main.hpp
)

add_subdirectory(temp)
add_executable(arcade ${SRC})
target_include_directories(arcade PUBLIC AddLib)
set_target_properties(arcade PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})