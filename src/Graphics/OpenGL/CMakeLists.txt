find_package(PkgConfig REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(Freetype REQUIRED)
find_package(glm REQUIRED)

set(OPENGL_UTILS_SRC
    OpenGLUtils/OpenGLRenderer.cpp
    OpenGLUtils/OpenGLText.cpp
    OpenGLUtils/OpenGLTexture.cpp
    OpenGLUtils/OpenGLWindow.cpp
    OpenGLUtils/OpenGLEvent.cpp
)

add_library(arcade_opengl SHARED
    OpenGL.cpp
    ${OPENGL_UTILS_SRC}
)

set_target_properties(arcade_opengl PROPERTIES PREFIX "" OUTPUT_NAME "arcade_opengl")
target_include_directories(arcade_opengl PUBLIC
    ${GLEW_INCLUDE_DIRS}
    ${OPENGL_INCLUDE_DIR}
    ${FREETYPE_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_CURRENT_SOURCE_DIR}
)

if(ENABLE_COVERAGE)
    target_compile_options(arcade_opengl PUBLIC -O0 -g --coverage -fprofile-arcs -ftest-coverage)
    target_link_libraries(arcade_opengl PUBLIC
        arcade_event
        arcade_ecs
        GLEW::GLEW
        ${OPENGL_LIBRARIES}
        glfw
        ${FREETYPE_LIBRARIES}
        --coverage
    )
else()
    target_link_libraries(arcade_opengl PUBLIC
        arcade_event
        arcade_ecs
        GLEW::GLEW
        ${OPENGL_LIBRARIES}
        glfw
        ${FREETYPE_LIBRARIES}
    )
endif()