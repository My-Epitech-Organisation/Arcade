#ifndef SRC_SHARED_INTERFACE_IJSONPARSER_HPP_
#define SRC_SHARED_INTERFACE_IJSONPARSER_HPP_

#include <string>
#include <vector>
#include <memory>
#include <map>
#include "Models/ColorType.hpp"

namespace Arcade {

// Graphical element structure
typedef struct graphical_element_s {
    std::string path;       // Path to the resource file (image, texture, etc.)
    std::string font;       // Font to use for text elements
    std::string text;       // Text content to display when isText is true
    bool isText;            // Flag to determine if the element is text
    char character;         // Character to display (for text or tile-based graphics)
    Color color;            // Color of the element
    float scale;            // Scale factor for the element
    float posX;             // X position on screen
    float posY;             // Y position on screen
    float rotation;         // Rotation angle in degrees
    bool isVisible;         // Flag to determine if the element should be rendered
    float width;            // Width of the element
    float height;           // Height of the element

    graphical_element_s() :
        path(""), font(""), text(""), isText(false), character(' '),
        color(NOCOLOR), scale(1.0f), posX(0.0f), posY(0.0f), rotation(0.0f),
        isVisible(true), width(0.0f), height(0.0f) {}
} graphical_element_t;

// JSON Value type enum
enum class JSONValueType {
    OBJECT,
    ARRAY,
    STRING,
    NUMBER,
    BOOLEAN,
    NULL_VALUE
};

// Forward declaration of JSONValue
class JSONValue;

// Interface for JSON parser
class IJSONParser {
public:
    virtual ~IJSONParser() = default;
    // Parse JSON from file
    virtual std::shared_ptr<JSONValue> parseFile(const std::string& filePath) = 0;
    // Parse JSON from string
    virtual std::shared_ptr<JSONValue> parseString(const std::string& jsonString) = 0;
    // Convert JSON asset configuration to graphical elements
    virtual std::map<std::string, graphical_element_t>
        convertAssetsToGraphicalElements(const std::string& jsonFilePath) = 0;
};

// JSON Value class to represent parsed JSON data
class JSONValue {
public:
    virtual ~JSONValue() = default;
    virtual JSONValueType getType() const = 0;
    virtual bool isObject() const = 0;
    virtual bool isArray() const = 0;
    virtual bool isString() const = 0;
    virtual bool isNumber() const = 0;
    virtual bool isBoolean() const = 0;
    virtual bool isNull() const = 0;
    virtual std::map<std::string, std::shared_ptr<JSONValue>> asObject() const = 0;
    virtual std::vector<std::shared_ptr<JSONValue>> asArray() const = 0;
    virtual std::string asString() const = 0;
    virtual double asNumber() const = 0;
    virtual bool asBoolean() const = 0;
    virtual bool hasKey(const std::string& key) const = 0;
    virtual std::shared_ptr<JSONValue> get(const std::string& key) const = 0;
    virtual std::shared_ptr<JSONValue> at(size_t index) const = 0;
};

}  // namespace Arcade

#endif  // SRC_SHARED_INTERFACE_IJSONPARSER_HPP_