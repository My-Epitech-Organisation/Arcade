set(SRCMAIN ${PROJECT_SOURCE_DIR}/src)

set(SRC
    ${SRCMAIN}/main.cpp
)

add_subdirectory(temp)
add_executable(arcade ${SRC})
target_include_directories(arcade PUBLIC AddLib)
set_target_properties(arcade PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})