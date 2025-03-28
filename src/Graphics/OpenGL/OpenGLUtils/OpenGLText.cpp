// Copyright 2025 <Epitech>
/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** OpenGLText
*/
#include <stdexcept>
#include <string>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "OpenGLUtils/OpenGLText.hpp"

namespace GL {

void OpenGLText::loadChar(FT_Face face) {
    for (unsigned char c = 0; c < 128; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }
        unsigned int texture;
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
            face->glyph->bitmap.buffer
        );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        Character character = {
            texture,
            static_cast<int>(face->glyph->bitmap.width),
            static_cast<int>(face->glyph->bitmap.rows),
            { face->glyph->bitmap_left, face->glyph->bitmap_top },
            static_cast<unsigned int>(face->glyph->advance.x)
        };
        _characters.insert(std::pair<char, Character>(c, character));
    }
}

void OpenGLText::init() {
    _shader = compileTextShader();

    FT_Library ft;
    if (FT_Init_FreeType(&ft))
        throw std::runtime_error("ERROR::FREETYPE: Could not init FreeType Library");

    FT_Face face;
    if (FT_New_Face(ft, "assets/fonts/arial.ttf", 0, &face))
        throw std::runtime_error("ERROR::FREETYPE: Failed to load font");
    FT_Set_Pixel_Sizes(face, 0, 24);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    FT_Done_Face(face);
    FT_Done_FreeType(ft);
    glGenVertexArrays(1, &_VAO);
    glGenBuffers(1, &_VBO);
    glBindVertexArray(_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void OpenGLText::drawText(int x, int y, const std::string& text,
    int windowWidth, int windowHeight) {
    glUseProgram(_shader);

    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(windowWidth),
        0.0f, static_cast<float>(windowHeight));
    unsigned int projectionLoc = glGetUniformLocation(_shader, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    unsigned int textColorLoc = glGetUniformLocation(_shader, "textColor");
    glUniform3f(textColorLoc, 1.0f, 1.0f, 1.0f);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(_VAO);
    y = windowHeight - y;
    float xpos = static_cast<float>(x);
    float scale = 1.0f;
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++) {
        Character ch = _characters[*c];
        float xpos_ch = xpos + ch.Bearing[0] * scale;
        float ypos_ch = y - (ch.Height - ch.Bearing[1]) * scale;
        float w = ch.Width * scale;
        float h = ch.Height * scale;
        float vertices[6][4] = {
            {xpos_ch, ypos_ch + h, 0.0f, 0.0f },
            {xpos_ch, ypos_ch, 0.0f, 1.0f },
            {xpos_ch + w, ypos_ch, 1.0f, 1.0f },
            {xpos_ch, ypos_ch + h, 0.0f, 0.0f },
            {xpos_ch + w, ypos_ch, 1.0f, 1.0f },
            {xpos_ch + w, ypos_ch + h, 1.0f, 0.0f }
        };

        glBindTexture(GL_TEXTURE_2D, ch.TextureID);

        glBindBuffer(GL_ARRAY_BUFFER, _VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        xpos += (ch.Advance >> 6) * scale;
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

unsigned int OpenGLText::compileTextShader() {
    const char* vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec4 vertex;\n"
        "out vec2 TexCoords;\n"
        "uniform mat4 projection;\n"
        "void main()\n"
        "{\n"
        "    gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);\n"
        "    TexCoords = vertex.zw;\n"
        "}\0";

    const char* fragmentShaderSource = "#version 330 core\n"
        "in vec2 TexCoords;\n"
        "out vec4 color;\n"
        "uniform sampler2D text;\n"
        "uniform vec3 textColor;\n"
        "void main()\n"
        "{\n"
        "    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);\n"
        "    color = vec4(textColor, 1.0) * sampled;\n"
        "}\0";

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    unsigned int shaderProgram = glCreateProgram();

    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, sizeof(infoLog), NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, sizeof(infoLog), NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, sizeof(infoLog), NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return shaderProgram;
}

}  // namespace GL
