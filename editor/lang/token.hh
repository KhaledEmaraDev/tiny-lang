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
    NONE
  };

  Token(Type type, int line, std::string lexeme, std::string literal)
      : m_type(type), m_line(line), m_lexeme(lexeme), m_literal(literal) {}

  Token() : Token(NONE, 0, "", "") {}

  Token(Type type, std::string lexeme) : m_type(type), m_lexeme(lexeme) {}

  friend std::ostream &operator<<(std::ostream &stream, const Token &token) {
    stream << token.m_lexeme << "," << token.m_type;

    return stream;
  }

  void set_token_type(Type type) {
    m_type = type;
  }

  void set_token_value(std::string lexeme) {
    m_lexeme = lexeme;
  }

  void set_token_literal(std::string literal) {
	m_literal = literal;
  }

  Type get_token_type() {
	  return m_type;
  }

  std::string get_token_value() {
	  return m_lexeme;
  }

  std::string get_token_literal() {
	  return m_literal;
  }

private:
  Type m_type;
  int m_line;
  std::string m_lexeme;
  std::string m_literal;
};
