#ifndef _TOKEN_H_
#define _TOKEN_H_

#include <iostream>
#include <string>

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

  Token() : m_type(NONE) {}

  Token(Type type)
      : m_type(type) {}

  Token(Type type, std::string lexeme)
      : m_type(type), m_lexeme(lexeme) {}


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

private:
  inline static const std::string type_name[] = {
      "IF",        "THEN",     "ELSE",        "END",
      "REPEAT",    "UNTIL",    "READ",        "WRITE",
      "PLUS",      "MINUS",    "MULT",        "DIV",
      "EQUAL",     "LESSTHAN", "OPENBRACKET", "CLOSEDBRACKET",
      "SEMICOLON", "ASSIGN",   "NUMBER",      "IDENTIFIER",
      "NONE"};
  Type m_type;
  std::string m_lexeme;
};

#endif
