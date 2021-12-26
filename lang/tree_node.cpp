#include <iostream>
#include <string>
#include <vector>

#include "treeNode.hh"

void TreeNode::add_child(TreeNode child) {
  this->m_children.push_back(new TreeNode(child.m_token_node));
  int size = this->m_children.size();

  // Adding children of the incoming node if exist.
  for (int i = 0; i < child.m_children.size(); ++i) {
    m_children[size - 1]->add_child(*child.m_children[i]);
  }

  // Adding sibling of the incoming node if exist.
  if (child.m_sibling != nullptr) {
    m_children[size - 1]->add_sibling(*child.m_sibling);
  }
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
void TreeNode::add_sibling(TreeNode sibling) {
  TreeNode *t_ptr = this;

  // Traverse through all existing siblings until we reach the last sibling.
  while (t_ptr->m_sibling != nullptr) {
    t_ptr = t_ptr->m_sibling;
  }

  // Adding the new incoming node as a sibling to the last existing one.
  t_ptr->m_sibling = new TreeNode(sibling.m_token_node);

  // Adding the children of the incoming node if exists.
  for (int i = 0; i < sibling.m_children.size(); i++) {
    t_ptr->m_sibling->add_child(*sibling.m_children[i]);
  }

  // In case the incoming node has already sibling, we add it.
  if (sibling.m_sibling != nullptr) {
    t_ptr->m_sibling->add_sibling(*sibling.m_sibling);
  }

  return;
}

void TreeNode::print_tree() {
  // Printing the token value and token node of the current node.
  std::cout << this->m_token_node.get_token_value() << ","
            << this->m_token_node.get_token_literal() << std::endl;

  // Printing the children of current node if exists.
  for (int i = 0; i < m_children.size(); ++i) {
    std::cout << "child number " << i << " of "
              << this->m_token_node.get_token_literal()
              << " is : " << m_children[i]->get_token().get_token_value() << ","
              << m_children[i]->get_token().get_token_literal() << std::endl;

    m_children[i]->print_tree();
  }

  // Printing the sibling of current node if exists.
  if (m_sibling != nullptr) {
    std::cout << "Sibling of " << this->m_token_node.get_token_literal()
              << " is : ";
    m_sibling->print_tree();
  }

  return;
}

TreeNode TreeNode::operator=(const TreeNode &copy_node) {
  this->m_token_node = copy_node.m_token_node;
  this->m_sibling = nullptr;
  this->m_children.clear();

  if (copy_node.m_sibling != nullptr) {
    this->add_sibling(*copy_node.m_sibling);
  }

  for (int i = 0; i < copy_node.m_children.size(); i++) {
    this->add_child(*copy_node.m_children[i]);
  }
  return *this;
}
