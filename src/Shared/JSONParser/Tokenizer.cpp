// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** B-OOP-400
** File description:
** Tokenizert
*/

#include "JSONParser/Tokenizer.hpp"
#include <stdexcept>
#include <cctype>

namespace Arcade {

Tokenizer::Tokenizer(const std::string& input)
    : _input(input), _position(0), _line(1), _column(1) {}

Token Tokenizer::nextToken() {
    skipWhitespace();
    if (isEnd()) {
        return Token(TokenType::END_OF_FILE, "", _line, _column);
    }
    char c = current();
    switch (c) {
        case '{':
            advance();
            return Token(TokenType::BEGIN_OBJECT, "{", _line, _column - 1);
        case '}':
            advance();
            return Token(TokenType::END_OBJECT, "}", _line, _column - 1);
        case '[':
            advance();
            return Token(TokenType::BEGIN_ARRAY, "[", _line, _column - 1);
        case ']':
            advance();
            return Token(TokenType::END_ARRAY, "]", _line, _column - 1);
        case ':':
            advance();
            return Token(TokenType::NAME_SEPARATOR, ":", _line, _column - 1);
        case ',':
            advance();
            return Token(TokenType::VALUE_SEPARATOR, ",", _line, _column - 1);
        case '"':
            return parseString();
        case '-':
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            return parseNumber();
        case 't':
        case 'f':
        case 'n':
            return parseIdentifier();
        default:
            throw std::runtime_error(
                "Unexpected character: " + std::string(1, c) +
                " at line " + std::to_string(_line) +
                ", column " + std::to_string(_column));
    }
}

Token Tokenizer::peekToken() {
    size_t savedPosition = _position;
    size_t savedLine = _line;
    size_t savedColumn = _column;
    Token token = nextToken();
    _position = savedPosition;
    _line = savedLine;
    _column = savedColumn;
    return token;
}

bool Tokenizer::hasMoreTokens() const {
    return !isEnd();
}

void Tokenizer::skipWhitespace() {
    while (!isEnd() && std::isspace(current())) {
        if (current() == '\n') {
            _line++;
            _column = 1;
        } else {
            _column++;
        }
        _position++;
    }
}

// Processes a string escape sequence after a backslash
char Tokenizer::processEscapeSequence() {
    if (isEnd()) {
        throw std::runtime_error
            ("Unexpected end of input in string escape sequence");
    }
    switch (current()) {
        case '"': return '"';
        case '\\': return '\\';
        case '/': return '/';
        case 'b': return '\b';
        case 'f': return '\f';
        case 'n': return '\n';
        case 'r': return '\r';
        case 't': return '\t';
        case 'u': {
            advance();
            advance(3);
            return '?';
        }
        default:
            throw std::runtime_error(
                "Invalid escape sequence: \\" + std::string(1, current()) +
                " at line " + std::to_string(_line) +
                ", column " + std::to_string(_column));
    }
}

Token Tokenizer::parseString() {
    size_t startColumn = _column;
    size_t startLine = _line;
    std::string value;
    // Skip opening quote
    advance();
    while (!isEnd() && current() != '"') {
        if (current() == '\\') {
            advance();
            value += processEscapeSequence();
        } else {
            value += current();
        }
        advance();
    }
    if (isEnd()) {
        throw std::runtime_error(
            "Unterminated string at line " + std::to_string(startLine) +
            ", column " + std::to_string(startColumn));
    }
    // Skip closing quote
    advance();
    return Token(TokenType::STRING, value, startLine, startColumn);
}

// Parse the fractional part of a number
void Tokenizer::parseFractionalPart(std::string& value) {
    value += current();
    advance();
    bool hasDigits = false;
    while (!isEnd() && std::isdigit(current())) {
        value += current();
        advance();
        hasDigits = true;
    }
    if (!hasDigits) {
        throw std::runtime_error(
            "Expected digit after decimal point at line " +
            std::to_string(_line) + ", column " + std::to_string(_column));
    }
}

// Parse the exponent part of a number
void Tokenizer::parseExponentPart(std::string& value) {
    value += current();
    advance();
    if (!isEnd() && (current() == '+' || current() == '-')) {
        value += current();
        advance();
    }
    bool hasDigits = false;
    while (!isEnd() && std::isdigit(current())) {
        value += current();
        advance();
        hasDigits = true;
    }
    if (!hasDigits) {
        throw std::runtime_error(
            "Expected digit in exponent at line " +
            std::to_string(_line) + ", column " + std::to_string(_column));
    }
}

Token Tokenizer::parseNumber() {
    size_t startColumn = _column;
    size_t startLine = _line;
    std::string value;

    // Handle negative sign
    if (current() == '-') {
        value += current();
        advance();
    }
    // Integer part
    while (!isEnd() && std::isdigit(current())) {
        value += current();
        advance();
    }
    // Fractional part
    if (!isEnd() && current() == '.') {
        parseFractionalPart(value);
    }
    // Exponent part
    if (!isEnd() && (current() == 'e' || current() == 'E')) {
        parseExponentPart(value);
    }
    return Token(TokenType::NUMBER, value, startLine, startColumn);
}

// Check if current token matches the pattern at current position
bool Tokenizer::matchesPattern(const std::string& pattern) {
    if (_position + pattern.length() <= _input.size() &&
        _input.substr(_position, pattern.length()) == pattern) {
        return true;
    }
    return false;
}

Token Tokenizer::parseIdentifier() {
    size_t startColumn = _column;
    size_t startLine = _line;
    if (current() == 't') {
        if (matchesPattern("true")) {
            advance(4);
            return Token(TokenType::BOOLEAN_TRUE, "true",
                startLine, startColumn);
        }
    } else if (current() == 'f') {
        if (matchesPattern("false")) {
            advance(5);
            return Token(TokenType::BOOLEAN_FALSE, "false",
                startLine, startColumn);
        }
    } else if (current() == 'n') {
        if (matchesPattern("null")) {
            advance(4);
            return Token(TokenType::NULL_VALUE, "null",
                startLine, startColumn);
        }
    }
    throw std::runtime_error(
        "Invalid identifier at line " + std::to_string(startLine) +
        ", column " + std::to_string(startColumn));
}

void Tokenizer::advance(size_t count) {
    for (size_t i = 0; i < count && !isEnd(); ++i) {
        if (current() == '\n') {
            _line++;
            _column = 1;
        } else {
            _column++;
        }
        _position++;
    }
}

char Tokenizer::current() const {
    if (isEnd()) {
        return '\0';
    }
    return _input[_position];
}

bool Tokenizer::isEnd() const {
    return _position >= _input.size();
}

char Tokenizer::peek(size_t offset) const {
    size_t pos = _position + offset;
    if (pos >= _input.size()) {
        return '\0';
    }
    return _input[pos];
}

}  // namespace Arcade
