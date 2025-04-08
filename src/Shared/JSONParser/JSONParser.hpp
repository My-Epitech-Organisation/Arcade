#ifndef SRC_SHARED_JSONPARSER_JSONPARSER_HPP_
#define SRC_SHARED_JSONPARSER_JSONPARSER_HPP_

#include <string>
#include <memory>
#include <map>
#include <vector>
#include "Interface/IJSONParser.hpp"
#include "JSONParser/Tokenizer.hpp"

namespace Arcade {

// Concrete implementation of JSONValue
class JSONValueImpl : public JSONValue {
 public:
    // Constructors for different value types
    static std::shared_ptr<JSONValueImpl> createObject(
        const std::map<std::string, std::shared_ptr<JSONValue>>& value);
    static std::shared_ptr<JSONValueImpl> createArray(
        const std::vector<std::shared_ptr<JSONValue>>& value);
    static std::shared_ptr<JSONValueImpl> createString(const std::string& value);
    static std::shared_ptr<JSONValueImpl> createNumber(double value);
    static std::shared_ptr<JSONValueImpl> createBoolean(bool value);
    static std::shared_ptr<JSONValueImpl> createNull();

    // Implementation of JSONValue interface
    JSONValueType getType() const override { return _type; }
    
    bool isObject() const override { return _type == JSONValueType::OBJECT; }
    bool isArray() const override { return _type == JSONValueType::ARRAY; }
    bool isString() const override { return _type == JSONValueType::STRING; }
    bool isNumber() const override { return _type == JSONValueType::NUMBER; }
    bool isBoolean() const override { return _type == JSONValueType::BOOLEAN; }
    bool isNull() const override { return _type == JSONValueType::NULL_VALUE; }
    
    std::map<std::string, std::shared_ptr<JSONValue>> asObject() const override;
    std::vector<std::shared_ptr<JSONValue>> asArray() const override;
    std::string asString() const override;
    double asNumber() const override;
    bool asBoolean() const override;
    
    bool hasKey(const std::string& key) const override;
    std::shared_ptr<JSONValue> get(const std::string& key) const override;
    std::shared_ptr<JSONValue> at(size_t index) const override;

 public:
    explicit JSONValueImpl(JSONValueType type) : _type(type) {}
    
    JSONValueType _type;
    std::map<std::string, std::shared_ptr<JSONValue>> _objectValue;
    std::vector<std::shared_ptr<JSONValue>> _arrayValue;
    std::string _stringValue;
    double _numberValue;
    bool _boolValue;
};

// Implementation of IJSONParser interface
class JSONParser : public IJSONParser {
 public:
    JSONParser() = default;
    ~JSONParser() override = default;
    
    std::shared_ptr<JSONValue> parseFile(const std::string& filePath) override;
    std::shared_ptr<JSONValue> parseString(const std::string& jsonString) override;
    
    std::map<std::string, graphical_element_t> 
        convertAssetsToGraphicalElements(const std::string& jsonFilePath) override;

 private:
    // Core parsing methods
    std::shared_ptr<JSONValue> parse(Tokenizer& tokenizer);
    std::shared_ptr<JSONValue> parseValue(Tokenizer& tokenizer);
    std::shared_ptr<JSONValue> parseObject(Tokenizer& tokenizer);
    std::shared_ptr<JSONValue> parseArray(Tokenizer& tokenizer);
    
    // Validation helper
    void validateTokenType(
        const Token& token, TokenType expected, const std::string& errorMsg);
    
    // Asset conversion helper methods
    void validateAssetsStructure(const std::shared_ptr<JSONValue>& jsonRoot);
    std::string extractFontPath(const std::shared_ptr<JSONValue>& assets);
    graphical_element_t createGraphicalElement(
        const std::shared_ptr<JSONValue>& node, const std::string& fontPath);
    
    void processTextures(
        const std::shared_ptr<JSONValue>& textures,
        std::map<std::string, graphical_element_t>& elements,
        const std::string& fontPath);
    
    void processTextureGroup(
        const std::shared_ptr<JSONValue>& group,
        std::map<std::string, graphical_element_t>& elements,
        const std::string& fontPath,
        const std::string& groupName);
    
    void processObjectTextureGroup(
        const std::shared_ptr<JSONValue>& group,
        std::map<std::string, graphical_element_t>& elements,
        const std::string& fontPath,
        const std::string& groupName);
    
    void processArrayTextureGroup(
        const std::shared_ptr<JSONValue>& group,
        std::map<std::string, graphical_element_t>& elements,
        const std::string& fontPath,
        const std::string& groupName);
};

}  // namespace Arcade

#endif  // SRC_SHARED_JSONPARSER_JSONPARSER_HPP_