#ifndef SRC_SHARED_JSONPARSER_TOKENIZER_HPP_
#define SRC_SHARED_JSONPARSER_TOKENIZER_HPP_

#include <string>
#include <vector>

namespace Arcade {

// Token types for JSON parsing
enum class TokenType {
    BEGIN_OBJECT,   // '{'
    END_OBJECT,     // '}'
    BEGIN_ARRAY,    // '['
    END_ARRAY,      // ']'
    NAME_SEPARATOR, // ':'
    VALUE_SEPARATOR,// ','
    STRING,         // "string"
    NUMBER,         // 123, 123.456
    BOOLEAN_TRUE,   // true
    BOOLEAN_FALSE,  // false
    NULL_VALUE,     // null
    END_OF_FILE     // End of input
};

// Token representation with position information
struct Token {
    TokenType type;
    std::string value;
    size_t line;
    size_t column;
    
    Token(TokenType type, const std::string& value, size_t line, size_t column)
        : type(type), value(value), line(line), column(column) {}
};

// JSON Tokenizer for breaking input into tokens
class Tokenizer {
 public:
    // Constructor takes JSON input string
    explicit Tokenizer(const std::string& input);
    
    // Get the next token from the input
    Token nextToken();
    
    // Peek at the next token without consuming it
    Token peekToken();
    
    // Check if there are more tokens to consume
    bool hasMoreTokens() const;
    
    // Get current line number
    size_t getCurrentLine() const { return _line; }
    
    // Get current column number
    size_t getCurrentColumn() const { return _column; }

 private:
    std::string _input;    // Input string being tokenized
    size_t _position;      // Current position in input
    size_t _line;          // Current line number
    size_t _column;        // Current column number
    
    // Skip whitespace characters
    void skipWhitespace();
    
    // Parse a JSON string
    Token parseString();
    
    // Parse a JSON number
    Token parseNumber();
    
    // Parse identifiers (true, false, null)
    Token parseIdentifier();
    
    // Advance the current position by count characters
    void advance(size_t count = 1);
    
    // Get current character
    char current() const;
    
    // Check if we've reached the end of input
    bool isEnd() const;
    
    // Peek at character at offset without advancing
    char peek(size_t offset = 0) const;
    
    // Process an escape sequence in a string
    char processEscapeSequence();
    
    // Parse the fractional part of a number
    void parseFractionalPart(std::string& value);
    
    // Parse the exponent part of a number
    void parseExponentPart(std::string& value);
    
    // Check if current position matches a pattern
    bool matchesPattern(const std::string& pattern);
};

}  // namespace Arcade

#endif  // SRC_SHARED_JSONPARSER_TOKENIZER_HPP_