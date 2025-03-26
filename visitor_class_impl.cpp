#include <string>
#include <string_view>
#include <format>
#include <vector>
#include <memory>
#include <iostream>

class NonTerminal;
class Token;

class Visitor {
public:
	virtual void visit(const NonTerminal&) = 0;
	virtual void visit(const Token&) = 0;

	std::string dump() {
		std::string temp(std::move(buffer_)); //clear and debug
		return temp;
	}
protected:
	std::string buffer_;
};
 
class LanguageCharacter {
public:
	virtual void accept(Visitor& visitor) const = 0;
};

class NonTerminal : public LanguageCharacter {
	static constexpr auto _class_name = "NonTerminal"; 
public:
	NonTerminal(std::string_view val) : non_terminal_(val) {}
	std::string_view GetCharacter() const {
		return non_terminal_;
	}
	std::string_view GetName() const { //TODO move to virtual
		return _class_name;
	}
	void accept(Visitor& visitor) const override {
		visitor.visit(*this);
	};
private:
	std::string non_terminal_;
};

class Token : public LanguageCharacter {
	static constexpr auto _class_name = "Token";
public:
	Token(std::string_view val) : token_(val) {}
	std::string_view GetCharacter() const {
		return token_;
	}
	std::string_view GetName() const {
		return _class_name;
	}
	void accept(Visitor& visitor) const override {
		visitor.visit(*this);
	};
private:
	std::string token_;
};

class JsonTranslator : public  Visitor {
public:
	//ISSUE can migrate to common function -> the same body of functions
	void visit(const NonTerminal& elem) override {
		buffer_ += std::format("{{\"{}\":{}}}\n", elem.GetName(), elem.GetCharacter());
	}
	void visit(const Token& elem) override {
		buffer_ += std::format("{{\"{}\":{}}}\n", elem.GetName(), elem.GetCharacter());
	}

};

class XmlTranslator : public  Visitor {
public:
	//ISSUE can migrate to common function -> the same body of functions
	void visit(const NonTerminal& elem) override {
		buffer_ += std::format("<{}>{}</{}>\n", elem.GetName(), elem.GetCharacter(), elem.GetName());
	}
	void visit(const Token& elem) override {
		buffer_ += std::format("<{}>{}</{}>\n", elem.GetName(), elem.GetCharacter(), elem.GetName());
	}
};

using character_collection_t = std::vector<std::unique_ptr<LanguageCharacter>>;

void simple_for_each(const character_collection_t& collection, Visitor& visitor) {
	for (const auto& elem : collection) {
		elem->accept(visitor);
	}
}

int main() {
	std::vector<std::unique_ptr<LanguageCharacter>> collection;
	collection.reserve(4);

	collection.emplace_back(std::make_unique<NonTerminal>("nonterminal_1"));
	collection.emplace_back(std::make_unique<NonTerminal>("nonterminal_2"));
	collection.emplace_back(std::make_unique<Token>("token_1"));
	collection.emplace_back(std::make_unique<Token>("token_2"));

	//JSON
	JsonTranslator json_translator;
	simple_for_each(collection, json_translator);

	//XML
	XmlTranslator xml_translator;
	simple_for_each(collection, xml_translator);

	std::cout << "###JSON_TRANSLATOR DATA###" << std::endl;;
	std::cout << json_translator.dump() << std::endl ;

	std::cout << std::endl;

	std::cout << "###XML_TRANSLATOR DATA###" << std::endl;;
	std::cout << xml_translator.dump() << std::endl;;

    return 0;
}