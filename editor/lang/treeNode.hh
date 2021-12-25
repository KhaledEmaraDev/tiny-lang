#pragma once

#include "token.hh"
#include <vector>

class TreeNode {
  
public:
  static int nodesCount;
  TreeNode() : m_sibling(nullptr) {}

  TreeNode(Token token)
      : m_token_node(token), m_sibling(nullptr) {}

  TreeNode(Token::Type token_type, std::string token_value)
      : m_sibling(nullptr) {
    Token token;
    token.set_token_type(token_type);
    token.set_token_value(token_value);
    nodesCount++;
    id = nodesCount;
    m_token_node = token;
  }

  void add_child(TreeNode child);
  void add_sibling(TreeNode sibling);
  void print_tree();

  void set_token(Token token) { m_token_node = token; }

  void set_token(Token::Type type, std::string value) {
    Token token;
    token.set_token_type(type);
    token.set_token_value(value);
    m_token_node = token;
  }

  void set_shape(std::string shape) { m_shape = shape; }

  TreeNode operator=(const TreeNode &copy_node);
  Token get_token() { return m_token_node; }
  std::vector<TreeNode *> get_node_children() {return m_children;}

  
  // We need to fix add_child function to take pointer
  // Will access private members directly for now

// private:
  Token m_token_node;
  std::vector<TreeNode *> m_children;
  TreeNode *m_sibling;
  std::string m_shape;
  int id;
};
