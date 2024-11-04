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
  ExprTree ret = NULL;


  //
  // TODO: Add your code here
  //

  return ret;
}


static ExprTree multiplicative(CList tokens, char *errmsg, size_t errmsg_sz)
{
  ExprTree ret = NULL;


  //
  // TODO: Add your code here
  //

  return ret;
}


static ExprTree exponential(CList tokens, char *errmsg, size_t errmsg_sz)
{
  ExprTree ret = NULL;


  //
  // TODO: Add your code here
  //

  return ret;
}


static ExprTree primary(CList tokens, char *errmsg, size_t errmsg_sz)
{
  ExprTree ret = NULL;

  if (TOK_next_type(tokens) == TOK_VALUE) {
    ret = ET_value(TOK_next(tokens).value);
    TOK_consume(tokens);

  } else if (TOK_next_type(tokens) == TOK_OPEN_PAREN) {


  //
  // TODO: Add your code here
  //

  } else if (TOK_next_type(tokens) == TOK_MINUS) {
    TOK_consume(tokens);

    ret = primary(tokens, errmsg, errmsg_sz);
    if (ret == NULL) 
      return NULL;
    ret = ET_node(UNARY_NEGATE, ret, NULL);

  } else {

  //
  // TODO: Add your code here
  //
  }


  return ret;
}


ExprTree Parse(CList tokens, char *errmsg, size_t errmsg_sz)
{

  //
  // TODO: Add your code here
  //
}
