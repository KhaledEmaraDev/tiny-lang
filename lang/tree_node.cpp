#include <iostream>
#include <string>
#include <vector>

#include "tree_node.h"


void TreeNode::add_child(TreeNode * child) {
    m_children.push_back(child);
}

/*
 * Example:
 *   read fact;
 *   fact := fact - 1
 *   end
 * Syntax Tree:
 *   read ------ assign
 *                  |
 *                  op
 *                 /  \
 *                id   number
 */
void TreeNode::set_sibling(TreeNode * sibling) {
    m_sibling = sibling;
}

void TreeNode::print() {
  // Printing the token value and token node of the current node.
  std::cout << m_token << std::endl;

  // Printing the children of current node if exists.
  for (int i = 0; i < m_children.size(); ++i) {
    std::cout << "child number " << i << " of " << m_token
              << " is : " << m_children[i]->token() << std::endl;

    m_children[i]->print();
  }

  // Printing the sibling of current node if exists.
  if (m_sibling != nullptr) {
    std::cout << "Sibling of " << m_token << " is : ";
    m_sibling->print();
  }

  return;
}

TreeNode::~TreeNode() {
    delete m_sibling;
    m_sibling = nullptr;
    for(auto child : m_children) {
        delete child;
        child = nullptr;
    }
}

