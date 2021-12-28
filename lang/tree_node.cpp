#include <iostream>
#include <queue>
#include <stack>
#include <string>
#include <vector>
#include <QTextStream>

#include "tree_node.h"

uint64_t TreeNode::id_counter = 0;

void TreeNode::add_child(TreeNode *child) { m_children.push_back(child); }

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
void TreeNode::set_sibling(TreeNode *sibling) { m_sibling = sibling; }

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

QString TreeNode::dot_representation() {
  QString buffer;
  QTextStream stream(&buffer);

  std::queue<TreeNode *> q;
  q.push(this);

  stream << "graph G { \n";

  while (!q.empty()) {
    TreeNode * top = q.front();
    q.pop();
    QString label = "\\n(" + QString::fromStdString(top->token().value()) + ")";
    QString shape = " shape=\"" + QString::fromStdString(top->shape()) + "\"";

    stream << top->id() << " [label= \"" << QString::fromStdString(labels[top->token().type()])
           << label << "\"" << shape << "];" << "\n";

    TreeNode * sibling = top->sibling();
    if (sibling != nullptr) {
      q.push(sibling);
      stream << top->id() << " -- " << sibling->id() << ";" << "\n";
      stream << "{rank = same; " << top->id() << "; " << sibling->id() << ";}\n";
    }

    std::vector<TreeNode*> top_children = top->children();
    for (TreeNode * child :  top_children) {
      stream << top->id() << " -- " << child->id() << ";" << "\n";
      q.push(child);
    }

    // for order
    if(top_children.size() > 1) {
      for (int i = 1; i < top_children.size(); i++) {
        stream << top_children[i - 1]->id() << " -- " << top_children[i]->id() << " [style=invis];\n";
        stream << "{rank = same; " << top_children[i - 1]->id() << "; " << top_children[i]->id() << ";}\n";
      }
    }

  }
  stream << "}";
  return stream.readAll();
}

TreeNode::~TreeNode() {
  delete m_sibling;
  m_sibling = nullptr;
  for (auto child : m_children) {
    delete child;
    child = nullptr;
  }
}
