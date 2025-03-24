set(EMROOT ${CMAKE_CURRENT_SOURCE_DIR}/EventManager)
set(INTROOT ${CMAKE_CURRENT_SOURCE_DIR}/Interface)
set(MODELROOT ${CMAKE_CURRENT_SOURCE_DIR}/Models)

set(SRC
    ${EMROOT}/AEventManager.cpp
)

set(INC
    ${EMROOT}/AEventManager.hpp
)

add_library(arcade_event STATIC ${SRC} ${INC})
target_include_directories(arcade_event PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
if(ENABLE_COVERAGE)
    target_compile_options(arcade_event PRIVATE -O0 -g --coverage -fprofile-arcs -ftest-coverage)
    target_link_libraries(arcade_event PRIVATE --coverage)
endif()
