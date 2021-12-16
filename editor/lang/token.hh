#pragma once

#include <ostream>
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
  };

  Token(Type type, int line, std::string lexeme, std::string literal)
      : m_type(type), m_line(line), m_lexeme(lexeme), m_literal(literal) {}

  friend std::ostream &operator<<(std::ostream &stream, const Token &token) {
    stream << token.m_lexeme << "," << token.m_type;

    return stream;
  }

  Type get_token_type() {
	  return m_type;
  }

  std::string get_token_value() {
	  return m_lexeme;
  }

private:
  const Type m_type;
  const int m_line;
  const std::string m_lexeme;
  const std::string m_literal;
};
