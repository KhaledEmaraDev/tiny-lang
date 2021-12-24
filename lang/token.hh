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

    // Find better way !!
    if (type == Token::Type::IF) {
      m_literal = "if";
    } else if (type == Token::Type::THEN) {
      m_literal = "then";
    } else if (type == Token::Type::ELSE) {
      m_literal = "else";
    } else if (type == Token::Type::END) {
      m_literal = "end";
    } else if (type == Token::Type::REPEAT) {
      m_literal = "repeat";
    } else if (type == Token::Type::UNTIL) {
      m_literal = "until";
    } else if (type == Token::Type::READ) {
      m_literal = "read";
    } else if (type == Token::Type::WRITE) {
      m_literal = "write";
    } else if (type == Token::Type::PLUS) {
      m_literal = "plus";
    } else if (type == Token::Type::MINUS) {
      m_literal = "minus";
    } else if (type == Token::Type::MULT) {
      m_literal = "mult";
    } else if (type == Token::Type::DIV) {
      m_literal = "div";
    } else if (type == Token::Type::EQUAL) {
      m_literal = "equal";
    } else if (type == Token::Type::LESSTHAN) {
      m_literal = "lessthan";
    } else if (type == Token::Type::SEMICOLON) {
      m_literal = "semicolon";
    } else if (type == Token::Type::OPENBRACKET) {
      m_literal = "openbracket";
    } else if (type == Token::Type::CLOSEDBRACKET) {
      m_literal = "closedbracket";
    } else if (type == Token::Type::ASSIGN) {
      m_literal = "assign";
    } else {
      m_literal = "identifier";
    }
  }

  void set_token_value(std::string lexeme) { m_lexeme = lexeme; }

  Type get_token_type() { return m_type; }

  std::string get_token_literal() { return m_literal; }

  std::string get_token_value() { return m_lexeme; }

private:
  Type m_type;
  int m_line;
  std::string m_lexeme;
  std::string m_literal;
};
