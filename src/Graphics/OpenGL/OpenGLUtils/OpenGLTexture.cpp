// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** OpenGLTexture
*/
#include <stdexcept>
#include <string>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "OpenGLUtils/stb_image.h"
#include "OpenGLUtils/OpenGLTexture.hpp"

namespace GL {

void OpenGLTexture::init() {
    // Initialize shaders after GLEW has been initialized
    initShaders();
    initRenderData();
}

unsigned int OpenGLTexture::loadTexture(const std::string& texturePath) {
    auto it = _textures.find(texturePath);
    if (it != _textures.end())
        return it->second;
    
    // Generate texture
    unsigned int textureID;
    glGenTextures(1, &textureID);
    
    // Load image
    int width, height, nrChannels;
    unsigned char* data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);
    if (data) {
        GLenum format;
        if (nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;
        
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        
        // Set texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        stbi_image_free(data);
        _textures[texturePath] = textureID;
    } else {
        std::cerr << "Failed to load texture: " << texturePath << std::endl;
        stbi_image_free(data);
        return 0;
    }
    if (_quadVAO == 0)
        initRenderData();
    
    return textureID;
}

void OpenGLTexture::initShaders() {
    const char *vertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec4 vertex; // <position, texCoords>
        
        out vec2 TexCoords;
        
        uniform mat4 projection;
        uniform vec2 position;
        uniform vec2 size;
        
        void main() {
            // Apply position and size transformations
            gl_Position = projection * vec4(
                position.x + vertex.x * size.x, 
                position.y + vertex.y * size.y, 
                0.0, 1.0);
            TexCoords = vertex.zw;
        }
    )";
    
    const char *fragmentShaderSource = R"(
        #version 330 core
        in vec2 TexCoords;
        out vec4 color;
        
        uniform sampler2D image;
        
        void main() {
            color = texture(image, TexCoords);
        }
    )";
    
    // Compile shaders
    unsigned int vertexShader, fragmentShader;
    
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    
    // Create program
    _shaderProgram = glCreateProgram();
    glAttachShader(_shaderProgram, vertexShader);
    glAttachShader(_shaderProgram, fragmentShader);
    glLinkProgram(_shaderProgram);
    
    // Clean up
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void OpenGLTexture::renderTexture(int x, int y, unsigned int textureID,
    int windowWidth, int windowHeight) {
        if (textureID == 0 || _shaderProgram == 0)
        return;
    
    // Use shader
    glUseProgram(_shaderProgram);
    
    // Set up orthographic projection
    float projection[16] = {
        2.0f/windowWidth, 0.0f, 0.0f, 0.0f,
        0.0f, -2.0f/windowHeight, 0.0f, 0.0f,
        0.0f, 0.0f, -1.0f, 0.0f,
        -1.0f, 1.0f, 0.0f, 1.0f
    };
    
    // Set uniform values
    GLint projLoc = glGetUniformLocation(_shaderProgram, "projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, projection);
    
    GLint posLoc = glGetUniformLocation(_shaderProgram, "position");
    glUniform2f(posLoc, (float)x, (float)y);
    
    GLint sizeLoc = glGetUniformLocation(_shaderProgram, "size");
    glUniform2f(sizeLoc, 100.0f, 100.0f); // Adjust size as needed
    
    // Set texture uniform
    glUniform1i(glGetUniformLocation(_shaderProgram, "image"), 0);
    
    // Bind texture and VAO
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glBindVertexArray(_quadVAO);
    
    // Render quad
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    // Cleanup
    glBindVertexArray(0);
    glUseProgram(0);
}

void OpenGLTexture::initRenderData() {
    // Configure VAO/VBO
    float vertices[] = { 
        // pos      // tex
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,
        
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };
    
    glGenVertexArrays(1, &_quadVAO);
    glGenBuffers(1, &_quadVBO);
    
    glBindBuffer(GL_ARRAY_BUFFER, _quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glBindVertexArray(_quadVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

}  // namespace GL
