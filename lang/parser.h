#ifndef _PARSER_H_
#define _PARSER_H_

#include <vector>

#include "tree_node.h"

class Parser {
public:
  Parser(std::vector<Token> tokens) : m_tokens(tokens) {}

  TreeNode* parse();

private:
  TreeNode* stmt_sequence();
  TreeNode* statement();
  TreeNode* if_stmt();
  TreeNode* exp();
  TreeNode* repeat_stmt();
  TreeNode* assign_stmt();
  TreeNode* read_stmt();
  TreeNode* write_stmt();
  TreeNode* comparison_op();
  TreeNode* simple_exp();
  TreeNode* add_op();
  TreeNode* term();
  TreeNode* mul_op();
  TreeNode* factor();

  void match(Token::Type expected_type) {
    if (m_tokens[m_idx].type() == expected_type) {
      advance();
    } else {
      fail(expected_type);
    }
  }

  void fail() {
      throw std::make_pair(
        m_tokens[m_idx].line(),
        "Unexpected token '" + m_tokens[m_idx].value() + "'"
      );
  }

  void fail(Token::Type expected_type) {
      throw std::make_pair(
        m_tokens[m_idx].line(),
        "Unexpected token '" + m_tokens[m_idx].value() + "', " +
        "Expected '" + Token(expected_type).literal() + "'"
      );
  }

  void advance() {
    if (m_idx < m_tokens.size() - 1) {
      ++m_idx;
    }
  }

private:
  std::vector<Token> m_tokens;
  int m_idx = 0;
};

#endif
