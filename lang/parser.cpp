#include "parser.h"
#include "token.h"

#include <iostream>

TreeNode * Parser::parse() {
  return stmt_sequence();
}

TreeNode * Parser::stmt_sequence() {
  TreeNode * node = statement();
  TreeNode * current_node = node;

  while (m_tokens[m_idx].type() == Token::Type::SEMICOLON) {
    match(Token::Type::SEMICOLON);
    current_node->set_sibling(statement());
    current_node = current_node->sibling();
  }

  return node;
}

TreeNode * Parser::statement() {
  TreeNode * node;
  Token::Type token_type = m_tokens[m_idx].type();

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
  } else {
    fail();
  }

  return node;
}

TreeNode * Parser::if_stmt() {
  TreeNode * if_node = new TreeNode(m_tokens[m_idx], "RECTANGLE");

  match(Token::Type::IF);
  if_node->add_child(exp());
  match(Token::Type::THEN);
  if_node->add_child(stmt_sequence());

  Token::Type token_type = m_tokens[m_idx].type();

  if (token_type == Token::Type::END) {
    advance();
  } else if (token_type == Token::Type::ELSE) {
    TreeNode * else_node = new TreeNode(m_tokens[m_idx], "RECTANGLE");

    advance();
    else_node->add_child(stmt_sequence());

    if_node->add_child(else_node);
    match(Token::Type::END);
  } else {
    fail();
  }

  return if_node;
}

TreeNode * Parser::repeat_stmt() {
  TreeNode * node = new TreeNode(m_tokens[m_idx], "RECTANGLE");

  match(Token::Type::REPEAT);
  node->add_child(stmt_sequence());
  match(Token::Type::UNTIL);
  node->add_child(exp());

  return node;
}

TreeNode * Parser::assign_stmt() {
  // I not sure but shouldn't it be
  // TreeNode * node = new TreeNode(Token(Token::Type::ASSIGN, ":="), "RECTANGLE");
  // node->add_child(new TreeNode(m_tokens[m_idx], "ELLIPSE"))
  // however
  Token mysterious_token(Token::Type::ASSIGN, m_tokens[m_idx].value());
  TreeNode * node = new TreeNode(mysterious_token, "RECTANGLE");

  match(Token::Type::IDENTIFIER);
  match(Token::Type::ASSIGN);

  node->add_child(exp());

  return node;
}

TreeNode * Parser::read_stmt() {
  match(Token::Type::READ);
  Token mysterious_token(Token::Type::READ, m_tokens[m_idx].value());
  TreeNode * node = new TreeNode(mysterious_token, "RECTANGLE");

  // Again shouldn't it be
  // TreeNode * node = new TreeNode(m_tokens[m_idx], "RECTANGLE");
  // match(Token::Type::READ);
  // node->add_child(new TreeNode(m_tokens[m_idx], "ELLIPSE"));

  match(Token::Type::IDENTIFIER);

  return node;
}

TreeNode * Parser::write_stmt() {
  TreeNode * node = new TreeNode(m_tokens[m_idx], "RECTANGLE");

  match(Token::Type::WRITE);
  node->add_child(exp());
  return node;
}

TreeNode * Parser::exp() {
  TreeNode * exp_node = simple_exp();

  Token::Type token_type = m_tokens[m_idx].type();

  if (token_type == Token::Type::LESSTHAN || token_type == Token::Type::EQUAL) {
    TreeNode * node = comparison_op();
    node->add_child(exp_node);
    node->add_child(simple_exp());
    exp_node = node;
  }

  return exp_node;
}

TreeNode * Parser::comparison_op() {
  TreeNode * node = new TreeNode(m_tokens[m_idx], "ELLIPSE");

  Token::Type token_type = m_tokens[m_idx].type();

  if (token_type == Token::Type::LESSTHAN) {
    advance();
  } else if (token_type == Token::Type::EQUAL) {
    advance();
  } else {
    advance();
  }

  return node;
}

TreeNode * Parser::simple_exp() {
  TreeNode * term_node = term();

  Token::Type token_type = m_tokens[m_idx].type();

  while (token_type == Token::Type::PLUS || token_type == Token::Type::MINUS) {
    TreeNode * node = add_op();
    node->add_child(term_node);
    node->add_child(term());
    term_node = node;
    token_type = m_tokens[m_idx].type();
  }
  return term_node;
}

TreeNode * Parser::add_op() {
  TreeNode * node = new TreeNode(m_tokens[m_idx], "ELLIPSE");

  Token::Type token_type = m_tokens[m_idx].type();

  if (token_type == Token::Type::PLUS) {
    advance();
  } else if (token_type == Token::Type::MINUS) {
    advance();
  } else {
    fail();
  }

  return node;
}

TreeNode * Parser::term() {
  TreeNode * factor_node = factor();

  Token::Type token_type = m_tokens[m_idx].type();

  while (token_type == Token::Type::MULT || token_type == Token::Type::DIV) {
    TreeNode * node = mul_op();
    node->add_child(factor_node);
    node->add_child(factor());
    factor_node = node;
    token_type = m_tokens[m_idx].type();
  }

  return factor_node;
}

TreeNode * Parser::mul_op() {
  TreeNode * node = new TreeNode(m_tokens[m_idx], "ELLIPSE");

  Token::Type token_type = m_tokens[m_idx].type();

  if (token_type == Token::Type::MULT) {
    advance();
  } else if (token_type == Token::Type::DIV) {
    advance();
  } else {
    fail();
  }

  return node;
}

TreeNode * Parser::factor() {
  TreeNode * node;
  Token::Type token_type = m_tokens[m_idx].type();

  if (token_type == Token::Type::OPENBRACKET) {
    advance();
    node = exp();
    match(Token::Type::CLOSEDBRACKET);
  } else if (token_type == Token::Type::NUMBER) {
    node = new TreeNode(m_tokens[m_idx], "ELLIPSE");
    advance();
  } else if (token_type == Token::Type::IDENTIFIER) {
    node = new TreeNode(m_tokens[m_idx], "ELLIPSE");
    advance();
  } else {
     fail();
  }

  return node;
}
