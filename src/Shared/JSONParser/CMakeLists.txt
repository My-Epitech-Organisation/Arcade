set(PARSROOT ${CMAKE_CURRENT_SOURCE_DIR}/../JSONParser)

set(SRC
    ${PARSROOT}/JSONParser.cpp
    ${PARSROOT}/Tokenizer.cpp
)

set(INC
    ${PARSROOT}/JSONParser.hpp
    ${PARSROOT}/Tokenizer.hpp
)

add_library(arcade_parser STATIC ${SRC} ${INC})
target_include_directories(arcade_parser PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/src)
if(ENABLE_COVERAGE)
    target_compile_options(arcade_parser PRIVATE -O0 -g --coverage -fprofile-arcs -ftest-coverage)
    target_link_libraries(arcade_parser PRIVATE --coverage)
else()
    target_link_libraries(arcade_parser PRIVATE)
endif()