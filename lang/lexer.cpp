#include "lexer.h"
#include "token.h"

#include <algorithm>
#include <cctype>
#include <string>
#include <unordered_map>

const std::unordered_map<std::string, Token::Type> Lexer::keywords = {
    {"if", Token::Type::IF},         {"then", Token::Type::THEN},
    {"else", Token::Type::ELSE},     {"end", Token::Type::END},
    {"repeat", Token::Type::REPEAT}, {"until", Token::Type::UNTIL},
    {"read", Token::Type::READ},     {"write", Token::Type::WRITE},
};

void Lexer::scan_token() {
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
    match_comment();
    break;

  case ':':
    if (match('='))
      push_token_back(Token::Type::ASSIGN);
    else
      log_error(m_current_line,
                "Unexpected operator: \":" + std::string(1, next()) + "\"");
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
      log_error(m_current_line,
                "Unexpected character: \"" + std::string(1, c) + "\"");
    break;
  }
}

void Lexer::match_identifier() {
  while (std::isalpha(peek()))
    next();

  std::string text =
      m_source_code.substr(m_start_idx, m_current_idx - m_start_idx);
  std::transform(text.begin(), text.end(), text.begin(),
                 [](char c) { return std::tolower(c); });

  Token::Type type = Token::Type::IDENTIFIER;

  auto type_it = keywords.find(text);
  if (type_it != keywords.end())
    type = type_it->second;

  push_token_back(type);
}

void Lexer::match_number() {
  while (std::isdigit(peek()))
    next();

  std::string text =
      m_source_code.substr(m_start_idx, m_current_idx - m_start_idx);

  push_token_back(Token::Type::NUMBER, text);
}

void Lexer::match_comment() {
  while (peek() != '}' && !eof()) {
    if (peek() == '\n')
      m_current_line++;
    next();
  }

  if (eof()) {
    log_error(m_current_line, "Unterminated comment.");
    return;
  }

  next();
}
