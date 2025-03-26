#include <iostream>
#include <string>
#include <variant>
#include <vector>
#include <format>

struct NonTerminal {
    std::string value;
    NonTerminal(const std::string& val) : value(val) {}
};

struct Token {
    std::string value;
    Token(const std::string& val) : value(val) {}
};

using LanguageCharacter = std::variant<Token, NonTerminal>;

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

void simple_for_each(const std::vector<LanguageCharacter>& collection, auto&& visitor) {
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
    simple_for_each(collection, json_translator);

    XmlTranslator xml_translator;
    simple_for_each(collection, xml_translator);

    std::cout << "###JSON_TRANSLATOR DATA###\n";
    std::cout << json_translator.dump() << std::endl;

    std::cout << "###XML_TRANSLATOR DATA###\n";
    std::cout << xml_translator.dump() << std::endl;

    return 0;
}