// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** B-OOP-400
** File description:
** IJSONParser interface
*/

#ifndef SRC_SHARED_INTERFACE_IJSONPARSER_HPP_
#define SRC_SHARED_INTERFACE_IJSONPARSER_HPP_

#include <string>
#include <vector>
#include <memory>
#include <map>
#include "Shared/JSONParser/JSONParser.hpp"
#include "ECS/Components/Drawable/DrawableComponent.hpp"

namespace Arcade {

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
    virtual std::shared_ptr<JSONValue>
        parseFile(const std::string& filePath) = 0;
    // Parse JSON from string
    virtual std::shared_ptr<JSONValue>
        parseString(const std::string& jsonString) = 0;
    // Convert JSON asset configuration to graphical elements
    virtual std::map<std::string, DrawableComponent>
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
    virtual std::map<std::string, std::shared_ptr<JSONValue>>
        asObject() const = 0;
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
