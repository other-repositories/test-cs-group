#include <iostream>
#include <string>
#include <variant>
#include <vector>
#include <format>

//TOKENS

struct NonTerminal {
    std::string value;
    NonTerminal(const std::string& val) : value(val) {}
};

struct Token {
    std::string value;
    Token(const std::string& val) : value(val) {}
};

using LanguageCharacter = std::variant<Token, NonTerminal>; //add your token

template<typename Visitor>
concept HasTokenAndNonTerminalOperators = requires(Visitor visitor, Token token, NonTerminal nonTerminal) {
    { visitor(token) } -> std::same_as<void>;          
    { visitor(nonTerminal) } -> std::same_as<void>;   
    //add your token
};

//FUNCTORS

struct JsonTranslator {
    void operator()(const Token& token) {
        buffer_ += std::format("{{\"Token\":\"{}\"}}\n", token.value);
    }

    void operator()(const NonTerminal& nonTerminal) {
        buffer_ += std::format("{{\"NonTerminal\":\"{}\"}}\n", nonTerminal.value);
    }

    std::string dump() const {
        std::string temp(std::move(buffer_)); //clear and debug
        return temp;
    }
private:
    std::string buffer_;
};

struct XmlTranslator {
    void operator()(const Token& token) {
        buffer_ += std::format("<Token>{}</Token>\n", token.value);
    }

    void operator()(const NonTerminal& nonTerminal) {
        buffer_ += std::format("<NonTerminal>{}</NonTerminal>\n", nonTerminal.value);
    }

    std::string dump() const {
        std::string temp(std::move(buffer_)); //clear and debug
        return temp;
    }
private:
    std::string buffer_;
};

void for_each_collection(const std::vector<LanguageCharacter>& collection, HasTokenAndNonTerminalOperators auto& visitor) {
    for (const auto& elem : collection) {
        std::visit(visitor, elem);  
    }
}

int main() {
    std::vector<LanguageCharacter> collection;
    collection.push_back(Token("token_1"));
    collection.push_back(NonTerminal("nonterminal_1"));
    collection.push_back(Token("token_2"));
    collection.push_back(NonTerminal("nonterminal_2"));

    JsonTranslator json_translator;
    for_each_collection(collection, json_translator);

    XmlTranslator xml_translator;
    for_each_collection(collection, xml_translator);

    //for_each_collection(collection, "25235"); compile error

    std::cout << "###JSON_TRANSLATOR DATA###\n";
    std::cout << json_translator.dump() << std::endl;

    std::cout << "###XML_TRANSLATOR DATA###\n";
    std::cout << xml_translator.dump() << std::endl;

    return 0;
}