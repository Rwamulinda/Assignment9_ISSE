/*
 * parse.c
 * 
 * Code that implements a recursive descent parser for arithmetic
 * expressions
 *
 * Author: <Uwase Pauline>
 */

#include <stdio.h>

#include "parse.h"
#include "tokenize.h"

/*
 * Forward declarations for the functions (rules) to produce the
 * ExpressionWhizz grammar.  See the assignment writeup for the grammar.
 * Each function has the same signature, so we will document all of
 * them here.
 *
 * Parameters:
 *   tokens     List of tokens remaining to be parsed
 *   errmsg     Return space for an error message, filled in in case of error
 *   errmsg_sz  The size of errmsg
 * 
 * Returns: The parsed ExprTree on success. If a parsing error is
 *   encountered, copies an error message into errmsg and returns
 *   NULL.
 */
static ExprTree additive(CList tokens, char *errmsg, size_t errmsg_sz);
static ExprTree multiplicative(CList tokens, char *errmsg, size_t errmsg_sz);
static ExprTree exponential(CList tokens, char *errmsg, size_t errmsg_sz);
static ExprTree primary(CList tokens, char *errmsg, size_t errmsg_sz);


static ExprTree additive(CList tokens, char *errmsg, size_t errmsg_sz)
{
  //ExprTree ret = NULL;

  ExprTree ret = multiplicative(tokens, errmsg, errmsg_sz);
  if (ret == NULL) 
    return NULL;

  while (TOK_next_type(tokens) == TOK_PLUS || TOK_next_type(tokens) == TOK_MINUS) {
    TokenType op = TOK_next_type(tokens);
    TOK_consume(tokens);  // Consume '+' or '-'
    ExprTree right = multiplicative(tokens, errmsg, errmsg_sz);
    if (right == NULL) {
        ET_free(ret);
        return NULL;
    }

    if(op == TOK_PLUS)
      ret = ET_node(OP_ADD, ret, right);
    else
      ret = ET_node(OP_SUB, ret, right);
  }

  return ret;
}


static ExprTree multiplicative(CList tokens, char *errmsg, size_t errmsg_sz)
{
  //ExprTree ret = NULL;


  ExprTree ret = exponential(tokens, errmsg, errmsg_sz);
  if (ret == NULL) 
      return NULL;

  while (TOK_next_type(tokens) == TOK_MULTIPLY || TOK_next_type(tokens) == TOK_DIVIDE) {
      TokenType op = TOK_next_type(tokens);
      TOK_consume(tokens);  // Consume '*' or '/'
  
      ExprTree right = exponential(tokens, errmsg, errmsg_sz);
      if (right == NULL) {
          ET_free(ret);
          return NULL;
      }

      if(op == TOK_MULTIPLY)
        ret = ET_node(OP_MUL, ret, right);
      else
        ret = ET_node(OP_DIV, ret, right);
    }



  
  return ret;
}


static ExprTree exponential(CList tokens, char *errmsg, size_t errmsg_sz)
{
  //ExprTree ret = NULL;

  ExprTree ret = primary(tokens, errmsg, errmsg_sz);
  if (ret == NULL) 
      return NULL;

  while (TOK_next_type(tokens) == TOK_POWER) {
      TOK_consume(tokens);  // Consume '^'
      ExprTree right = exponential(tokens, errmsg, errmsg_sz);  // Right associative
      if (right == NULL) {
          ET_free(ret);
          return NULL;
        }
        ret = ET_node(OP_POWER, ret, right);
    }



  
  return ret;
}


static ExprTree primary(CList tokens, char *errmsg, size_t errmsg_sz)
{
  ExprTree ret = NULL;

  if (TOK_next_type(tokens) == TOK_VALUE) {
    ret = ET_value(TOK_next(tokens).value);
    TOK_consume(tokens);

  } else if (TOK_next_type(tokens) == TOK_OPEN_PAREN) {

      TOK_consume(tokens);  // Consume '('
      ret = additive(tokens, errmsg, errmsg_sz);  // Parse inner expression
      if (TOK_next_type(tokens) == TOK_CLOSE_PAREN) {
          TOK_consume(tokens);  // Consume ')'
      } else {
          snprintf(errmsg, errmsg_sz, "Unexpected token %s", TT_to_str(TOK_next_type(tokens)));
          ET_free(ret);
          return NULL;
        }
  //

  } else if (TOK_next_type(tokens) == TOK_MINUS) {
    TOK_consume(tokens);

    ret = primary(tokens, errmsg, errmsg_sz);
    if (ret == NULL) 
      return NULL;
    ret = ET_node(UNARY_NEGATE, ret, NULL);

  } else {
    snprintf(errmsg, errmsg_sz, "Unexpected token %s", TT_to_str(TOK_next(tokens).type));
    return NULL;
  }


  return ret;
}


ExprTree Parse(CList tokens, char *errmsg, size_t errmsg_sz)
{
  ExprTree tree = additive(tokens, errmsg, errmsg_sz);

  if (tree == NULL) {
    return NULL;  // Error already set in errmsg
  }

    // Ensure we have reached the end of input
  if (TOK_next_type(tokens) != TOK_END) {
      snprintf(errmsg, errmsg_sz, "Syntax error on token %s", TT_to_str(TOK_next_type(tokens)));
      ET_free(tree);
      return NULL;
  }
  return tree;
}
