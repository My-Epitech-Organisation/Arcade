// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** OpenGLTexture
*/

#include "OpenGLUtils/OpenGLTexture.hpp"
#include <iostream>
#include <vector>
#include <string>
#define STB_IMAGE_IMPLEMENTATION
#include "OpenGLUtils/stb_image.h"

namespace GL {

OpenGLTexture::OpenGLTexture() : _VAO(0), _VBO(0),
_shaderProgram(0), _isInitialized(false) {}

OpenGLTexture::~OpenGLTexture() {
    for (auto &texture : _textures)
        glDeleteTextures(1, &texture.second.id);

    if (_isInitialized) {
        glDeleteBuffers(1, &_VBO);
        glDeleteVertexArrays(1, &_VAO);
        glDeleteProgram(_shaderProgram);
    }
}

void OpenGLTexture::init() {
    if (_isInitialized)
        return;

    const char* vertexShaderSource = R"glsl(
        #version 330 core
        layout (location = 0) in vec4 vertex; // <position, texCoords>
        out vec2 TexCoords;
        uniform mat4 model;
        uniform mat4 projection;

        void main()
        {
            gl_Position = projection * model * vec4(vertex.xy, 0.0, 1.0);
            TexCoords = vertex.zw;
        }
    )glsl";

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
            << infoLog << std::endl;
        return;
    }

    const char* fragmentShaderSource = R"glsl(
        #version 330 core
        in vec2 TexCoords;
        out vec4 color;
        uniform sampler2D texture1;

        void main()
        {
            color = texture(texture1, TexCoords);
        }
    )glsl";

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
            << infoLog << std::endl;
        return;
    }

    _shaderProgram = glCreateProgram();
    glAttachShader(_shaderProgram, vertexShader);
    glAttachShader(_shaderProgram, fragmentShader);
    glLinkProgram(_shaderProgram);

    glGetProgramiv(_shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(_shaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
            << infoLog << std::endl;
        return;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glGenVertexArrays(1, &_VAO);
    glGenBuffers(1, &_VBO);

    glBindVertexArray(_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, _VBO);

    float vertices[] = {
        0.0f, 0.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 1.0f
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE,
        4 * sizeof(float), reinterpret_cast<void*>(0));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    _isInitialized = true;
}

bool OpenGLTexture::loadTexture(const std::string &filename,
const std::string &id) {
    if (_textures.find(id) != _textures.end())
        return true;

    stbi_set_flip_vertically_on_load(true);

    int width, height, channels;
    unsigned char *data = stbi_load(filename.c_str(), &width,
        &height, &channels, 0);
    if (!data) {
        std::cerr << "Failed to load texture: " << filename << std::endl;
        return false;
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
        GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    GLenum format = GL_RGB;
    if (channels == 1)
        format = GL_RED;
    else if (channels == 3)
        format = GL_RGB;
    else if (channels == 4)
        format = GL_RGBA;

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0,
        format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    TextureData textureData;
    textureData.id = textureID;
    textureData.width = width;
    textureData.height = height;
    _textures[id] = textureData;

    return true;
}

void OpenGLTexture::draw(int x, int y, const std::string &textureId) {
    if (!_isInitialized) {
        std::cerr << "Texture system not initialized" << std::endl;
        return;
    }

    auto it = _textures.find(textureId);
    if (it == _textures.end()) {
        if (!loadTexture(textureId, textureId)) {
            std::cerr << "Texture not found: " << textureId << std::endl;
            return;
        }
        it = _textures.find(textureId);
    }

    TextureData &texture = it->second;

    glUseProgram(_shaderProgram);

    int viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    float width = static_cast<float>(texture.width);
    float height = static_cast<float>(texture.height);

    float yPos = static_cast<float>(viewport[3] - y - height);
    float model[16] = {
        width, 0.0f, 0.0f, 0.0f,
        0.0f, height, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        static_cast<float>(x), yPos, 0.0f, 1.0f
    };

    float projection[16] = {
        2.0f / viewport[2], 0.0f, 0.0f, 0.0f,
        0.0f, 2.0f / viewport[3], 0.0f, 0.0f,
        0.0f, 0.0f, -1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f, 1.0f
    };

    GLint modelLoc = glGetUniformLocation(_shaderProgram, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model);

    GLint projLoc = glGetUniformLocation(_shaderProgram, "projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, projection);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture.id);

    glBindVertexArray(_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    glDisable(GL_BLEND);
    glBindTexture(GL_TEXTURE_2D, 0);
}

}  // namespace GL
