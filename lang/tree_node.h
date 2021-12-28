#ifndef _TREE_NODE_H_
#define _TREE_NODE_H_

#include "token.h"
#include <cstdint>
#include <vector>
#include <QString>

class TreeNode {
public:
  TreeNode() : m_sibling(nullptr) { m_id = ++id_counter; }
  ~TreeNode();

  TreeNode(Token token) : m_token(token), m_sibling(nullptr) {
    m_id = ++id_counter;
  }

  TreeNode(Token token, std::string shape)
      : m_token(token), m_sibling(nullptr), m_shape(shape) {
    m_id = ++id_counter;
  }

  void add_child(TreeNode *child);
  void set_sibling(TreeNode *sibling);
  void print();

  void set_token(Token token) { m_token = token; }

  void set_token(Token::Type type, std::string value) {
    m_token = Token(type, value);
  }

  void set_shape(std::string shape) { m_shape = shape; }

  Token token() { return m_token; }
  TreeNode *sibling() const { return m_sibling; };
  std::vector<TreeNode *> children() { return m_children; }
  std::string shape() { return m_shape; }
  uint64_t id() { return m_id; }

  QString dot_representation();

private:
  Token m_token;
  std::vector<TreeNode *> m_children;
  TreeNode *m_sibling;
  std::string m_shape;
  uint64_t m_id;
  static uint64_t id_counter;

  inline static const std::string labels[] = {
      "if",        "then",     "else",        "end",
      "repeat",    "until",    "read",        "write",
      "op",        "op",       "op",          "op",
      "op",        "op",       "open bracket", "closed bracket",
      "semicolon", "assign",   "const",      "id",
      "none"
  };
};

#endif
