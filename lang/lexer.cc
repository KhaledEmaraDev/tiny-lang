#include "lexer.hh"
#include "token.hh"

#include <algorithm>
#include <cctype>
#include <string>
#include <unordered_map>

const std::unordered_map<std::string, Token::Type> Lexer::keywords = {
    {"if", Token::Type::IF},           {"then", Token::Type::THEN},
    {"else", Token::Type::ELSE},       {"end", Token::Type::END},
    {"repeate", Token::Type::REPEATE}, {"until", Token::Type::UNTIL},
    {"read", Token::Type::READ},       {"write", Token::Type::WRITE},
};

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
