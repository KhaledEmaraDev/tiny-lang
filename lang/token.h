#ifndef _TOKEN_H_
#define _TOKEN_H_

#include <iostream>
#include <string>
#include <map>

class Token {
public:
    enum Type {
      // Reserved Words
      IF,
      THEN,
      ELSE,
      END,
      REPEAT,
      UNTIL,
      READ,
      WRITE,

      // Special Symbols
      PLUS,
      MINUS,
      MULT,
      DIV,
      EQUAL,
      LESSTHAN,
      OPENBRACKET,
      CLOSEDBRACKET,
      SEMICOLON,
      ASSIGN,

      // Other
      NUMBER,
      IDENTIFIER,
      NONE
    };

  Token() : m_type(NONE), m_line(1) {}

  Token(Type type)
      : m_type(type), m_line(1) {}

  Token(Type type, std::string lexeme, int line = 1)
      : m_type(type), m_lexeme(lexeme), m_line(line) {}


  friend std::ostream &operator<<(std::ostream &stream, const Token &token) {
    stream << token.m_lexeme << "," << Token::type_name[token.m_type];

    return stream;
  }

  void set_type(Type type) {
    m_type = type;
  }

  void set_value(std::string lexeme) { m_lexeme = lexeme; }

  Type type() { return m_type; }

  std::string literal() { return type_name[m_type]; }

  std::string value() { return m_lexeme; }

  int line() { return m_line; }

  static Type type_from_string(std::string type) {
    auto type_it = types_map.find(type);

    if (type_it == types_map.end())
        throw std::string("Couldn't parse type!");

    return type_it->second;
  }

private:
  inline static const std::string type_name[] = {
      "IF",        "THEN",     "ELSE",        "END",
      "REPEAT",    "UNTIL",    "READ",        "WRITE",
      "PLUS",      "MINUS",    "MULT",        "DIV",
      "EQUAL",     "LESSTHAN", "OPENBRACKET", "CLOSEDBRACKET",
      "SEMICOLON", "ASSIGN",   "NUMBER",      "IDENTIFIER",
      "NONE"};
  static const std::map<std::string, Token::Type> types_map;
  Type m_type;
  std::string m_lexeme;
  int m_line;
};

#endif
