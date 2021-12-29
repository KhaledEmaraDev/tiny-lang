#include "token.h"

const std::map<std::string, Token::Type> Token::types_map = {
    {"IF", Token::Type::IF},                    {"THEN", Token::Type::THEN},
    {"ELSE", Token::Type::ELSE},                {"END", Token::Type::END},
    {"REPEAT", Token::Type::REPEAT},            {"UNTIL", Token::Type::UNTIL},
    {"READ", Token::Type::READ},                {"WRITE", Token::Type::WRITE},
    {"PLUS", Token::Type::PLUS},                {"MINUS", Token::Type::MINUS},
    {"MULT", Token::Type::MULT},                {"DIV", Token::Type::DIV},
    {"EQUAL", Token::Type::EQUAL},              {"LESSTHAN", Token::Type::LESSTHAN},
    {"OPENBRACKET", Token::Type::OPENBRACKET},  {"CLOSEDBRACKET", Token::Type::CLOSEDBRACKET},
    {"SEMICOLON", Token::Type::SEMICOLON},      {"ASSIGN", Token::Type::ASSIGN},
    {"NUMBER", Token::Type::NUMBER},            {"IDENTIFIER", Token::Type::IDENTIFIER},
    {"NONE", Token::Type::NONE},
};
