#ifndef _TREE_NODE_H_
#define _TREE_NODE_H_

#include <vector>
#include "token.h"

class TreeNode {
public:
  TreeNode() : m_sibling(nullptr) {}
  ~TreeNode();

  TreeNode(Token token) : m_token(token), m_sibling(nullptr) {}
  TreeNode(Token token, std::string shape) : m_token(token), m_sibling(nullptr), m_shape(shape) {}


  void add_child(TreeNode * child);
  void set_sibling(TreeNode * sibling);
  void print();

  void set_token(Token token) { m_token = token; }

  void set_token(Token::Type type, std::string value) {
    m_token = Token(type, value);
  }

  void set_shape(std::string shape) { m_shape = shape; }

  Token token() { return m_token; }
  TreeNode *sibling() const {  return m_sibling; };
  std::vector<TreeNode *> children() { return m_children; }

private:
  Token m_token;
  std::vector<TreeNode *> m_children;
  TreeNode * m_sibling;
  std::string m_shape;
};

#endif
