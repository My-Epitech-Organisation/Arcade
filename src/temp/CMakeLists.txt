set(INCADD ${PROJECT_SOURCE_DIR}/src/temp)
set(SRCADD ${PROJECT_SOURCE_DIR}/src/temp)
set(INCROOT ${PROJECT_SOURCE_DIR}/src)

set(SRC
    ${SRCADD}/add.cpp
)

set(INC
    ${INCADD}/add.hpp
)

add_library(AddLib STATIC ${SRC} ${INC})
target_include_directories(AddLib PUBLIC ${INCROOT})

if(ENABLE_COVERAGE)
    target_compile_options(AddLib PRIVATE -O0 -g --coverage -fprofile-arcs -ftest-coverage)
    target_link_libraries(AddLib PRIVATE --coverage)
endif()