#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "lexer.hh"
#include "token.hh"

class Tiny {
public:
  Tiny(std::string source_code): m_source_code(source_code) {}

  std::vector<Token> get_tokens() {
      auto lexer = Lexer(m_source_code);
      return lexer.get_tokens();
  }

private:
  std::string m_source_code;
};
