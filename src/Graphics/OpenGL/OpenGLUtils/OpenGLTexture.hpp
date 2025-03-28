// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** OpenGLTexture
*/

#ifndef SRC_GRAPHICS_OPENGL_OPENGLUTILS_OPENGLTEXTURE_HPP_
    #define SRC_GRAPHICS_OPENGL_OPENGLUTILS_OPENGLTEXTURE_HPP_
    #include <GL/glew.h>
    #include <GLFW/glfw3.h>
    #include <memory>
    #include <string>
    #include <unordered_map>

namespace GL {

class OpenGLTexture {
 public:
    OpenGLTexture() : _quadVAO(0), _quadVBO(0), _shaderProgram(0) {}
    ~OpenGLTexture() = default;

    void init();
    unsigned int loadTexture(const std::string& texturePath);
    void renderTexture(int x, int y, unsigned int textureID, int windowWidth, int windowHeight);
    
 private:
    std::unordered_map<std::string, unsigned int> _textures;
    unsigned int _quadVAO = 0;
    unsigned int _quadVBO = 0;
    unsigned int _shaderProgram = 0;
    void initShaders();
    void initRenderData();
};

}  // namespace GL

#endif  // SRC_GRAPHICS_OPENGL_OPENGLUTILS_OPENGLTEXTURE_HPP_
