// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** OpenGLText
*/

#include "OpenGLUtils/OpenGLText.hpp"
#include <iostream>
#include <algorithm>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "OpenGL/OpenGLColor.hpp"

namespace GL {

OpenGLText::OpenGLText() : _VAO(0), _VBO(0), _shaderProgram(0),
_isInitialized(false), _fontHeight(0), _fontAscender(0) {}

OpenGLText::~OpenGLText() {
    if (_isInitialized) {
        for (auto &ch : _characters)
            glDeleteTextures(1, &ch.second.textureID);

        glDeleteBuffers(1, &_VBO);
        glDeleteVertexArrays(1, &_VAO);
        glDeleteProgram(_shaderProgram);
    }
}

void OpenGLText::init() {
    if (_isInitialized)
        return;

    const char* vertexShaderSource = R"glsl(
        #version 330 core
        layout (location = 0) in vec4 vertex; // <pos.x, pos.y, tex.x, tex.y>
        out vec2 TexCoords;
        uniform mat4 projection;

        void main()
        {
            gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);
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
        uniform sampler2D text;
        uniform vec4 textColor;

        void main()
        {
            vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
            color = textColor * sampled;
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
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog
        << std::endl;
        return;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glGenVertexArrays(1, &_VAO);
    glGenBuffers(1, &_VBO);

    glBindVertexArray(_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        std::cerr << "ERROR::FREETYPE: Could not init FreeType Library"
        << std::endl;
        return;
    }

    FT_Face face = nullptr;

    bool fontLoaded = false;
    if (FT_New_Face(ft, "./assets/fonts/arial.ttf", 0, &face) == 0) {
        std::cout << "Loaded font from: ./assets/fonts/arial.ttf" << std::endl;
        fontLoaded = true;
    }

    if (!fontLoaded || !face) {
        std::cerr << "ERROR::FREETYPE: Failed to load font" << std::endl;
        FT_Done_FreeType(ft);
        return;
    }

    FT_Set_Pixel_Sizes(face, 0, 24);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    int maxAscender = 0;
    int maxDescender = 0;
    int maxHeight = 0;

    for (unsigned char c = 32; c < 128; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            continue;
        maxAscender = std::max(maxAscender, face->glyph->bitmap_top);
        int descender = static_cast<int>(face->glyph->bitmap.rows)
            - face->glyph->bitmap_top;
        maxDescender = std::max(maxDescender, descender);
        maxHeight = std::max(maxHeight,
            static_cast<int>(face->glyph->bitmap.rows));
    }

    for (unsigned char c = 0; c < 128; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            std::cerr << "ERROR::FREETYPE: Failed to load Glyph for char "
                << static_cast<int>(c) << std::endl;
            continue;
        }

        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Character character = {
            texture,
            static_cast<int>(face->glyph->bitmap.width),
            static_cast<int>(face->glyph->bitmap.rows),
            face->glyph->bitmap_left,
            face->glyph->bitmap_top,
            static_cast<unsigned int>(face->glyph->advance.x)
        };
        _characters.insert(std::pair<char, Character>(c, character));
    }

    _fontHeight = maxAscender + maxDescender;
    _fontAscender = maxAscender;

    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    _isInitialized = true;
}

void OpenGLText::draw(const std::string &text, int x,
int y, Arcade::Color color) {
    if (!_isInitialized) {
        std::cerr << "Text system not initialized" << std::endl;
        return;
    }

    glUseProgram(_shaderProgram);

    auto glColor = OpenGLColor::convertColor(color);
    glUniform4f(
        glGetUniformLocation(_shaderProgram, "textColor"),
        glColor[0], glColor[1], glColor[2], glColor[3]);

    int viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    glm::mat4 projection = glm::ortho(
        0.0f, static_cast<float>(viewport[2]),
        static_cast<float>(viewport[3]), 0.0f);

    glUniformMatrix4fv(
        glGetUniformLocation(_shaderProgram, "projection"),
        1,
        GL_FALSE,
        glm::value_ptr(projection));

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBindVertexArray(_VAO);

    float xpos = static_cast<float>(x);
    float ypos = static_cast<float>(y) + _fontAscender;

    for (char c : text) {
        auto it = _characters.find(c);
        if (it == _characters.end()) {
            xpos += 8.0f;
            continue;
        }
        Character ch = it->second;

        float x1 = xpos + static_cast<float>(ch.bearingX);
        float y1 = ypos - static_cast<float>(ch.bearingY);
        float w = static_cast<float>(ch.width);
        float h = static_cast<float>(ch.height);

        float vertices[6][4] = {
            { x1, y1, 0.0f, 0.0f },
            { x1, y1 + h, 0.0f, 1.0f },
            { x1 + w, y1, 1.0f, 0.0f },
            { x1 + w, y1, 1.0f, 0.0f },
            { x1, y1 + h, 0.0f, 1.0f },
            { x1 + w, y1 + h, 1.0f, 1.0f }
        };

        glBindTexture(GL_TEXTURE_2D, ch.textureID);

        glBindBuffer(GL_ARRAY_BUFFER, _VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        xpos += static_cast<float>(ch.advance >> 6);
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_BLEND);
}

}  // namespace GL
