set(EMROOT ${CMAKE_CURRENT_SOURCE_DIR}/EventManager)
set(INTROOT ${CMAKE_CURRENT_SOURCE_DIR}/Interface)
set(MODELROOT ${CMAKE_CURRENT_SOURCE_DIR}/Models)
set(SYSROOT ${CMAKE_CURRENT_SOURCE_DIR}/System)
set(COMPROOT ${CMAKE_CURRENT_SOURCE_DIR}/Components)

set(SRC
    ${EMROOT}/AEventManager.cpp
    ${MODELROOT}/EntityComponentSystem/ComponentManager.cpp
    ${MODELROOT}/EntityComponentSystem/EntityManager.cpp
    ${MODELROOT}/SystemManager/SystemManager.cpp
    ${MODELROOT}/WindowManager/WindowManager.cpp
    ${SYSROOT}/MovementSystem.cpp
)

set(INC
    ${EMROOT}/AEventManager.hpp
    ${MODELROOT}/EntityComponentSystem/ComponentManager.hpp
    ${MODELROOT}/EntityComponentSystem/EntityManager.hpp
    ${MODELROOT}/SystemManager/SystemManager.hpp
    ${MODELROOT}/WindowManager/WindowManager.hpp
    ${SYSROOT}/MovementSystem.hpp
)

add_library(arcade_shared STATIC ${SRC} ${INC})
target_include_directories(arcade_shared PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
if(ENABLE_COVERAGE)
    target_compile_options(arcade_shared PRIVATE -O0 -g --coverage -fprofile-arcs -ftest-coverage)
    target_link_libraries(arcade_shared PRIVATE --coverage)
endif()
