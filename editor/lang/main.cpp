#include <bits/stdc++.h>
#include "token.hh"
#include "treeNode.hh"

void print_graph(TreeNode *root) {
    // Change to file output (fout)
    cout << "digraph G { " << endl;
    queue<TreeNode *> q;
    q.push(root);
    while (!q.empty()) {
        auto top = q.front();
        q.pop();
        string valueLabel = "";
        string nodeShape = "";

        if(top->get_token().get_token_value().length())
        {
            valueLabel="\\n(" +top->get_token().get_token_value() + ")";
        }
        if(top->get_token().get_token_type()==Token::Type::REPEAT || top->get_token().get_token_type()==Token::Type::ASSIGN)
        {
            nodeShape=" shape=\"square\"";
        }
        cout << top->id << "[label= \"" << top->get_token().get_token_literal() << valueLabel<< "\"" << nodeShape<< "];" << endl;


        auto sibling = top->get_sibling();
        if (sibling != nullptr) {
            q.push(sibling);
            cout << top->id << " -> " << sibling->id << ";" << endl;
            cout << "{rank = same; " << top->id << "; " << sibling->id << ";}" << endl;
        }

        for (auto child: top->get_node_children()) {
            cout << top->id << " -> " << child->id << ";" << endl;
            q.push(child);
        }
    }
    cout << "}";
}

int TreeNode::nodesCount;

int main() {

    // Mocking input
    auto root = new TreeNode(Token::Type::REPEAT, "");
    auto ch1 = new TreeNode(Token::Type::ASSIGN, "fact");
    auto ch2 = new TreeNode(Token::Type::EQUAL, "=");
    auto s1 = new TreeNode(Token::Type::ASSIGN, "x");

    auto ch11 = new TreeNode(Token::Type::NUMBER, "3");
    auto ch12 = new TreeNode(Token::Type::NUMBER, "2");

    auto ch21 = new TreeNode(Token::Type::IDENTIFIER, "x");
    auto ch22 = new TreeNode(Token::Type::NUMBER, "0");

    root->m_children.push_back(ch1);
    root->m_children.push_back(ch2);
    ch1->m_sibling = s1;
    ch1->m_children.push_back(ch11);
    s1->m_children.push_back(ch12);

    ch2->m_children.push_back(ch21);
    ch2->m_children.push_back(ch22);


    print_graph(root);


    return 0;
}
