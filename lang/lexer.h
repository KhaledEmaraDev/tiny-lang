#pragma once

#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "parser.hh"
#include "token.hh"

class Lexer {
public:
  Lexer(std::string source) : m_source_code(source) {}

  std::vector<Token> get_tokens() {
    while (!eof()) {
      m_start_idx = m_current_idx;
      scan_token();
    }

    if (m_errors.size() > 0)
      throw m_errors;

    // for testing
    for (int i = 0; i < m_tokens.size(); ++i) {
      std::cout << m_tokens[i].get_token_value() << " "
                << m_tokens[i].get_token_literal() << std::endl;
    }
    auto parser = new Parser(m_tokens);
    TreeNode node = parser->program();
    node.print_tree();
    // End testing

    return m_tokens;
  }

private:
  void scan_token();
  void match_comment();
  void match_identifier();
  void match_number();

  char next() { return m_source_code[m_current_idx++]; }

  bool eof(int offset = 0) {
    return m_current_idx + offset >= m_source_code.length();
  }

  char peek(int offset = 0) {
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

  void push_token_back(Token::Type type) { push_token_back(type, ""); }

  void push_token_back(Token::Type type, std::string literal) {
    std::string lexeme =
        m_source_code.substr(m_start_idx, m_current_idx - m_start_idx);
    m_tokens.push_back(Token(type, m_current_line, lexeme, literal));
  }

  void log_error(int line, std::string message) {
    m_errors.push_back(std::make_pair(line, message));
  }

private:
  const std::string m_source_code;
  std::vector<Token> m_tokens;
  std::string::size_type m_start_idx = 0;
  std::string::size_type m_current_idx = 0;
  int m_current_line = 1;

  std::vector<std::pair<int, std::string>> m_errors;

  static const std::unordered_map<std::string, Token::Type> keywords;
};
