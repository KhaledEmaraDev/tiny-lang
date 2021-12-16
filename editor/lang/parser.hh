#pragma once

#include "treeNode.hh"
#include <vector>

class Parser {
public:
  Parser(std::vector<Token> tokens) : m_tokens(tokens) {}

  void match(Token::Type expected_type);

  TreeNode program();
  TreeNode stmt_sequence();
  TreeNode statement();
  TreeNode if_stmt();
  TreeNode exp();
  TreeNode repeat_stmt();
  TreeNode assign_stmt();
  TreeNode read_stmt();
  TreeNode write_stmt();
  TreeNode comparison_op();
  TreeNode simple_exp();
  TreeNode add_op();
  TreeNode term();
  TreeNode mul_op();
  TreeNode factor();

private:
  std::vector<Token> m_tokens;
  int m_current_idx = 0;

  void advance() {
    if (m_current_idx < m_tokens.size() - 1) {
      ++m_current_idx;
    }
  }
};