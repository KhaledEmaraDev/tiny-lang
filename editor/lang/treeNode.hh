#pragma once

#include "token.hh"
#include <vector>

class TreeNode {
public:
  TreeNode();
  TreeNode(Token token);
  TreeNode(std::string token_type, std::string token_value);

  void add_child(TreeNode child);
  void add_sibling(TreeNode sibling);
  void print_node();

  void set_token(Token token);
  void set_token(Token::Type type, std::string value);
  void set_shape(std::string shape);

  TreeNode operator=(const TreeNode &copy);
  Token get_token();

  private:
  Token m_token_node;
  std::vector<TreeNode *> m_children;
  TreeNode *m_sibling;
  std::string m_shape;
};