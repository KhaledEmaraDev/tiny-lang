#include "parser.hh"
#include "token.hh"

void Parser::match(Token::Type expected_type) {
  if (m_tokens[m_current_idx].get_token_type() == expected_type) {
    advance();
  }
}

TreeNode Parser::program() {
  TreeNode root_node = stmt_sequence();
  return root_node;
}

TreeNode Parser::stmt_sequence() {
  TreeNode node1, node2;
  node1 = statement();

  while (m_tokens[m_current_idx].get_token_type() == Token::Type::SEMICOLON) {
    match(Token::Type::SEMICOLON);
    node2 = statement();
    node1.add_sibling(node2);
  }

  return node1;
}

TreeNode Parser::statement() {
  TreeNode node;
  Token::Type token_type = m_tokens[m_current_idx].get_token_type();

  if (token_type == Token::Type::IF) {
    node = if_stmt();
  } else if (token_type == Token::Type::REPEAT) {
    node = repeat_stmt();
  } else if (token_type == Token::Type::IDENTIFIER) {
    node = assign_stmt();
  } else if (token_type == Token::Type::READ) {
    node = read_stmt();
  } else if (token_type == Token::Type::WRITE) {
    node = write_stmt();
  }

  return node;
}

TreeNode Parser::if_stmt() {
  TreeNode node, new_node;
  node.set_token(Token::Type::IF, "if");
  node.set_shape("RECTANGLE");

  match(Token::Type::IF);

  TreeNode exp_node = exp();
  node.add_child(exp_node);

  match(Token::Type::THEN);

  TreeNode stmt_node = stmt_sequence();
  node.add_child(stmt_node);

  Token::Type token_type = m_tokens[m_current_idx].get_token_type();

  if (token_type == Token::Type::END) {
    match(token_type);
  } else if (token_type == Token::Type::ELSE) {
    new_node.set_token(Token::Type::ELSE, "else");
    new_node.set_shape("RECTANGLE");

    match(Token::Type::ELSE);

    new_node.add_child(stmt_sequence());
    node.add_child(new_node);

    match(Token::Type::END);
  }

  return node;
}

TreeNode Parser::repeat_stmt() {
  TreeNode node;
  node.set_token(Token::Type::REPEAT, "repeat");
  node.set_shape("RECTANGLE");

  match(Token::Type::REPEAT);

  TreeNode stmt_node = stmt_sequence();
  node.add_child(stmt_node);

  match(Token::Type::UNTIL);

  TreeNode exp_node = exp();
  node.add_child(exp_node);

  return node;
}

TreeNode Parser::assign_stmt() {
  TreeNode node;
  std::string token_value = m_tokens[m_current_idx].get_token_value();
  node.set_token(Token::Type::ASSIGN, token_value);
  node.set_shape("RECTANGLE");

  match(Token::Type::IDENTIFIER);
  match(Token::Type::ASSIGN);

  TreeNode exp_node = exp();
  node.add_child(exp_node);

  return node;
}

TreeNode Parser::read_stmt() {
  TreeNode node;
  match(Token::Type::READ);
  std::string token_value = m_tokens[m_current_idx].get_token_value();

  node.set_token(Token::Type::READ, token_value);
  node.set_shape("RECTANGLE");

  match(Token::Type::IDENTIFIER);

  return node;
}

TreeNode Parser::write_stmt() {
  TreeNode node;
  node.set_token(Token::Type::WRITE, "write");
  node.set_shape("RECTANGLE");

  match(Token::Type::WRITE);

  TreeNode exp_node = exp();
  node.add_child(exp_node);
  return node;
}

TreeNode Parser::exp() {
  TreeNode node, new_node;
  node = simple_exp();

  Token::Type token_type = m_tokens[m_current_idx].get_token_type();

  if (token_type == Token::Type::LESSTHAN || token_type == Token::Type::EQUAL) {
    new_node = comparison_op();
    new_node.add_child(node);
    new_node.add_child(simple_exp());
    node = new_node;
  }

  return node;
}

TreeNode Parser::comparison_op() {
  TreeNode node;
  node.set_shape("ELLIPSE");

  Token::Type token_type = m_tokens[m_current_idx].get_token_type();

  if (token_type == Token::Type::LESSTHAN) {
    node.set_token(Token::Type::LESSTHAN, "<");
    match(Token::Type::LESSTHAN);
  } else if (token_type == Token::Type::EQUAL) {
    node.set_token(Token::Type::EQUAL, "=");
    match(Token::Type::EQUAL);
  }

  return node;
}

TreeNode Parser::simple_exp() {
  TreeNode node, new_node;
  node = term();

  Token::Type token_type = m_tokens[m_current_idx].get_token_type();

  while (token_type == Token::Type::PLUS || token_type == Token::Type::MINUS) {
    new_node = add_op();
    new_node.add_child(node);
    new_node.add_child(term());
    node = new_node;
  }
  return node;
}

TreeNode Parser::add_op() {
  TreeNode node;
  node.set_shape("ELLIPSE");

  Token::Type token_type = m_tokens[m_current_idx].get_token_type();

  if (token_type == Token::Type::PLUS) {
    match(Token::Type::PLUS);
    node.set_token(Token::Type::PLUS, "+");
  } else if (token_type == Token::Type::MINUS) {
    match(Token::Type::MINUS);
    node.set_token(Token::Type::MINUS, "-");
  }

  return node;
}

TreeNode Parser::term() {
  TreeNode node, new_node;
  node = factor();

  Token::Type token_type = m_tokens[m_current_idx].get_token_type();

  while (token_type == Token::Type::MULT || token_type == Token::Type::DIV) {
    new_node = mul_op();
    new_node.add_child(node);
    new_node.add_child(factor());
    node = new_node;
  }

  return node;
}

TreeNode Parser::mul_op() {
  TreeNode node;
  node.set_shape("ELLIPSE");

  Token::Type token_type = m_tokens[m_current_idx].get_token_type();

  if (token_type == Token::Type::MULT) {
    node.set_token(Token::Type::MULT, "*");
    match(Token::Type::MULT);
  } else if (token_type == Token::Type::DIV) {
    node.set_token(Token::Type::DIV, "/");
    match(Token::Type::DIV);
  }

  return node;
}

TreeNode Parser::factor() {
  TreeNode node;
  Token::Type token_type = m_tokens[m_current_idx].get_token_type();

  if (token_type == Token::Type::OPENBRACKET) {
    match(Token::Type::OPENBRACKET);
    node = exp();
    match(Token::Type::CLOSEDBRACKET);
  } else if (token_type == Token::Type::NUMBER) {
    std::string token_value = m_tokens[m_current_idx].get_token_value();
    node.set_token(Token::Type::NUMBER, token_value); 
    node.set_shape("ELLIPSE");
    match(Token::Type::NUMBER);
  } else if (token_type == Token::Type::IDENTIFIER) {
    std::string token_value = m_tokens[m_current_idx].get_token_value();
    node.set_token(Token::Type::IDENTIFIER, token_value); 
    node.set_shape("ELLIPSE");
    match(Token::Type::IDENTIFIER);
  }

  return node;
}
