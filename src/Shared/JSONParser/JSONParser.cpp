// Copyright <2025> Epitech
/*
** EPITECH PROJECT, 2025
** B-OOP-400
** File description:
** JSONParser
*/

#include "JSONParser/JSONParser.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <memory>

namespace Arcade {

// JSONValueImpl static factory methods
std::shared_ptr<JSONValueImpl> JSONValueImpl::createObject(
const std::map<std::string, std::shared_ptr<JSONValue>>& value) {
    auto obj = std::make_shared<JSONValueImpl>(JSONValueType::OBJECT);
    obj->_objectValue = value;
    return obj;
}

std::shared_ptr<JSONValueImpl> JSONValueImpl::createArray(
const std::vector<std::shared_ptr<JSONValue>>& value) {
    auto arr = std::make_shared<JSONValueImpl>(JSONValueType::ARRAY);
    arr->_arrayValue = value;
    return arr;
}

std::shared_ptr<JSONValueImpl> JSONValueImpl::createString
(const std::string& value) {
    auto str = std::make_shared<JSONValueImpl>(JSONValueType::STRING);
    str->_stringValue = value;
    return str;
}

std::shared_ptr<JSONValueImpl> JSONValueImpl::createNumber(double value) {
    auto num = std::make_shared<JSONValueImpl>(JSONValueType::NUMBER);
    num->_numberValue = value;
    return num;
}

std::shared_ptr<JSONValueImpl> JSONValueImpl::createBoolean(bool value) {
    auto boolean = std::make_shared<JSONValueImpl>(JSONValueType::BOOLEAN);
    boolean->_boolValue = value;
    return boolean;
}

std::shared_ptr<JSONValueImpl> JSONValueImpl::createNull() {
    return std::make_shared<JSONValueImpl>(JSONValueType::NULL_VALUE);
}

// JSONValueImpl interface implementation
std::map<std::string, std::shared_ptr<JSONValue>>
JSONValueImpl::asObject() const {
    if (!isObject()) {
        throw std::runtime_error("JSON value is not an object");
    }
    return _objectValue;
}

std::vector<std::shared_ptr<JSONValue>> JSONValueImpl::asArray() const {
    if (!isArray()) {
        throw std::runtime_error("JSON value is not an array");
    }
    return _arrayValue;
}

std::string JSONValueImpl::asString() const {
    if (!isString()) {
        throw std::runtime_error("JSON value is not a string");
    }
    return _stringValue;
}

double JSONValueImpl::asNumber() const {
    if (!isNumber()) {
        throw std::runtime_error("JSON value is not a number");
    }
    return _numberValue;
}

bool JSONValueImpl::asBoolean() const {
    if (!isBoolean()) {
        throw std::runtime_error("JSON value is not a boolean");
    }
    return _boolValue;
}

bool JSONValueImpl::hasKey(const std::string& key) const {
    if (!isObject()) {
        return false;
    }
    return _objectValue.find(key) != _objectValue.end();
}

std::shared_ptr<JSONValue> JSONValueImpl::get(const std::string& key) const {
    if (!isObject()) {
        throw std::runtime_error("JSON value is not an object");
    }
    auto it = _objectValue.find(key);
    if (it == _objectValue.end()) {
        throw std::runtime_error("Key not found: " + key);
    }
    return it->second;
}

std::shared_ptr<JSONValue> JSONValueImpl::at(size_t index) const {
    if (!isArray()) {
        throw std::runtime_error("JSON value is not an array");
    }
    if (index >= _arrayValue.size()) {
        throw std::runtime_error(
            "Array index out of bounds: " + std::to_string(index));
    }
    return _arrayValue[index];
}

// JSONParser implementation
std::shared_ptr<JSONValue> JSONParser::parseFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filePath);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return parseString(buffer.str());
}

std::shared_ptr<JSONValue> JSONParser::parseString
(const std::string& jsonString) {
    Tokenizer tokenizer(jsonString);
    return parse(tokenizer);
}

std::shared_ptr<JSONValue> JSONParser::parse(Tokenizer& tokenizer) {
    return parseValue(tokenizer);
}

std::shared_ptr<JSONValue> JSONParser::parseValue(Tokenizer& tokenizer) {
    Token token = tokenizer.nextToken();
    switch (token.type) {
        case TokenType::BEGIN_OBJECT:
            return parseObject(tokenizer);
        case TokenType::BEGIN_ARRAY:
            return parseArray(tokenizer);
        case TokenType::STRING:
            return JSONValueImpl::createString(token.value);
        case TokenType::NUMBER:
            return JSONValueImpl::createNumber(std::stod(token.value));
        case TokenType::BOOLEAN_TRUE:
            return JSONValueImpl::createBoolean(true);
        case TokenType::BOOLEAN_FALSE:
            return JSONValueImpl::createBoolean(false);
        case TokenType::NULL_VALUE:
            return JSONValueImpl::createNull();
        default:
            throw std::runtime_error(
                "Unexpected token: " + token.value +
                " at line " + std::to_string(token.line) +
                ", column " + std::to_string(token.column));
    }
}

// Helper to validate expected token type
void JSONParser::validateTokenType(
const Token& token, TokenType expected, const std::string& errorMsg) {
    if (token.type != expected) {
        throw std::runtime_error(
            errorMsg + " at line " + std::to_string(token.line) +
            ", column " + std::to_string(token.column));
    }
}

std::shared_ptr<JSONValue> JSONParser::parseObject(Tokenizer& tokenizer) {
    std::map<std::string, std::shared_ptr<JSONValue>> object;
    Token token = tokenizer.peekToken();
    if (token.type == TokenType::END_OBJECT) {
        tokenizer.nextToken();
        return JSONValueImpl::createObject(object);
    }
    while (true) {
        // Parse key
        Token keyToken = tokenizer.nextToken();
        validateTokenType(
            keyToken, TokenType::STRING, "Expected string key in object");
        std::string key = keyToken.value;

        // Parse colon
        Token colonToken = tokenizer.nextToken();
        validateTokenType(
            colonToken, TokenType::NAME_SEPARATOR,
            "Expected ':' after key in object");
        // Parse value
        object[key] = parseValue(tokenizer);
        // Check for comma or end of object
        Token separatorToken = tokenizer.nextToken();
        if (separatorToken.type == TokenType::END_OBJECT) {
            break;
        }
        validateTokenType(
            separatorToken, TokenType::VALUE_SEPARATOR,
            "Expected ',' or '}' in object");
    }
    return JSONValueImpl::createObject(object);
}

std::shared_ptr<JSONValue> JSONParser::parseArray(Tokenizer& tokenizer) {
    std::vector<std::shared_ptr<JSONValue>> array;
    Token token = tokenizer.peekToken();
    if (token.type == TokenType::END_ARRAY) {
        tokenizer.nextToken();
        return JSONValueImpl::createArray(array);
    }
    while (true) {
        // Parse value
        array.push_back(parseValue(tokenizer));
        // Check for comma or end of array
        Token separatorToken = tokenizer.nextToken();
        if (separatorToken.type == TokenType::END_ARRAY) {
            break;
        }
        validateTokenType(
            separatorToken, TokenType::VALUE_SEPARATOR,
            "Expected ',' or ']' in array");
    }
    return JSONValueImpl::createArray(array);
}

// Helper to extract font path from assets
std::string JSONParser::extractFontPath
(const std::shared_ptr<JSONValue>& assets) {
    if (assets->hasKey("fonts") && assets->get("fonts")->hasKey("default")) {
        return assets->get("fonts")->get("default")->asString();
    }
    return "./assets/fonts/Arial.ttf";
}

// Helper to validate assets structure
void JSONParser::validateAssetsStructure
(const std::shared_ptr<JSONValue>& jsonRoot) {
    if (!jsonRoot->isObject() || !jsonRoot->hasKey("assets")) {
        throw std::runtime_error
        ("Invalid JSON structure: missing 'assets' object");
    }
    auto assets = jsonRoot->get("assets");
    if (!assets->isObject()) {
        throw std::runtime_error("'assets' is not an object");
    }
    return;
}

std::map<std::string, DrawableComponent>
JSONParser::convertAssetsToGraphicalElements
(const std::string& jsonFilePath) {
    std::map<std::string, DrawableComponent> elements;
    try {
        auto jsonRoot = parseFile(jsonFilePath);
        validateAssetsStructure(jsonRoot);
        auto assets = jsonRoot->get("assets");
        std::string fontPath = extractFontPath(assets);
        if (assets->hasKey("textures")) {
            processTextures(assets->get("textures"), elements, fontPath);
        }
        return elements;
    } catch (const std::exception& e) {
        std::cerr << "Error converting assets to graphical elements: "
                  << e.what() << std::endl;
        return elements;
    }
}

void JSONParser::processTextures(
const std::shared_ptr<JSONValue>& textures,
std::map<std::string, DrawableComponent>& elements,
const std::string& fontPath) {
    if (!textures->isObject()) {
        throw std::runtime_error("'textures' is not an object");
    }
    auto textureMap = textures->asObject();
    for (const auto& [groupName, groupValue] : textureMap) {
        processTextureGroup(groupValue, elements, fontPath, groupName);
    }
}

// Create a graphical element from a texture node
DrawableComponent JSONParser::createGraphicalElement(
const std::shared_ptr<JSONValue>& node, const std::string& fontPath) {
    DrawableComponent element;
    element.path = node->get("path")->asString();
    element.font = fontPath;
    element.character = node->get("char")->asString()[0];
    // Use setAsTexture if width and height are specified
    if (node->hasKey("width") && node->hasKey("height")) {
        float width = node->get("width")->asNumber();
        float height = node->get("height")->asNumber();
        element.setAsTexture(element.path, width, height);
    } else {
        // Otherwise, set as character
        element.setAsCharacter(element.character);
    }
    return element;
}

// Process object texture nodes
void JSONParser::processObjectTextureGroup(
const std::shared_ptr<JSONValue>& group,
std::map<std::string, DrawableComponent>& elements,
const std::string& fontPath,
const std::string& groupName) {
    auto groupObj = group->asObject();
    // Check if this is a leaf texture node (has path and char)
    if (group->hasKey("path") && group->hasKey("char")) {
        elements[groupName] = createGraphicalElement(group, fontPath);
    } else {
        // This is a parent node, process its children
        for (const auto& [subName, subValue] : groupObj) {
            std::string fullName = groupName + "." + subName;
            processTextureGroup(subValue, elements, fontPath, fullName);
        }
    }
}

// Process array texture nodes
void JSONParser::processArrayTextureGroup(
const std::shared_ptr<JSONValue>& group,
std::map<std::string, DrawableComponent>& elements,
const std::string& fontPath,
const std::string& groupName) {
    auto arr = group->asArray();
    for (size_t i = 0; i < arr.size(); ++i) {
        std::string indexName = groupName + "[" + std::to_string(i) + "]";
        processTextureGroup(arr[i], elements, fontPath, indexName);
    }
}

void JSONParser::processTextureGroup(
const std::shared_ptr<JSONValue>& group,
std::map<std::string, DrawableComponent>& elements,
const std::string& fontPath,
const std::string& groupName) {
    if (group->isObject()) {
        processObjectTextureGroup(group, elements, fontPath, groupName);
    } else if (group->isArray()) {
        processArrayTextureGroup(group, elements, fontPath, groupName);
    }
}


}  // namespace Arcade
