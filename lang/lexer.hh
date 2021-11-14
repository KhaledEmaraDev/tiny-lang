#pragma once

#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <unordered_map>
#include <vector>

#include "logger.hh"
#include "token.hh"

class Lexer {
public:
  Lexer(std::string source) : m_source_code(source) {}

  std::vector<Token> get_tokens() {
    while (!eof()) {
      m_start_idx = m_current_idx;
      scan_token();
    }

    return m_tokens;
  }

private:
  void scan_token() {
    char c = next();

    switch (c) {
    case '+':
      push_token_back(Token::Type::PLUS);
      break;
    case '-':
      push_token_back(Token::Type::MINUS);
      break;
    case '*':
      push_token_back(Token::Type::MULT);
      break;
    case '/':
      push_token_back(Token::Type::DIV);
      break;
    case '=':
      push_token_back(Token::Type::EQUAL);
      break;
    case '<':
      push_token_back(Token::Type::LESSTHAN);
      break;
    case '(':
      push_token_back(Token::Type::OPENBRACKET);
      break;
    case ')':
      push_token_back(Token::Type::OPENBRACKET);
      break;
    case ';':
      push_token_back(Token::Type::SEMICOLON);
      break;

    case '{':
      while (peek() != '}' && !eof()) {
        if (peek() == '\n')
          m_current_line++;
        next();
      }

      if (eof()) {
        log_error(m_current_line, "Unterminated comment.");
        break;
      }

      next();
      break;

    case ':':
      if (match('='))
        push_token_back(Token::Type::ASSIGN);
      else
        log_error(m_current_line, "Unexpected character: " + std::string(1, c));
      break;

    case ' ':
    case '\r':
    case '\t':
      break;

    case '\n':
      m_current_line++;
      break;

    default:
      if (std::isdigit(c))
        match_number();
      else if (std::isalpha(c))
        match_identifier();
      else
        log_error(m_current_line, "Unexpected character: " + std::string(1, c));
      break;
    }
  }

  inline char next() { return m_source_code[m_current_idx++]; }

  inline bool eof(int offset = 0) {
    return m_current_idx + offset >= m_source_code.length();
  }

  inline char peek(int offset = 0) {
    if (eof(offset))
      return '\0';
    return m_source_code[m_current_idx + offset];
  }

  bool match(char expected) {
    if (eof())
      return false;
    if (m_source_code[m_current_idx] != expected)
      return false;

    m_current_idx++;
    return true;
  }

  inline void push_token_back(Token::Type type) { push_token_back(type, ""); }

  inline void push_token_back(Token::Type type, std::string literal) {
    std::string lexeme =
        m_source_code.substr(m_start_idx, m_current_idx - m_start_idx);
    m_tokens.push_back(Token(type, m_current_line, lexeme, literal));
  }

  void match_identifier();

  void match_number();

private:
  const std::string m_source_code;
  std::vector<Token> m_tokens;
  std::string::size_type m_start_idx = 0;
  std::string::size_type m_current_idx = 0;
  int m_current_line = 1;

  static const std::unordered_map<std::string, Token::Type> keywords;
};
