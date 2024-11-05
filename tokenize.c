/*
 * tokenize.c
 * 
 * Functions to tokenize and manipulate lists of tokens
 *
 * Author: <Uwase Pauline>
 */

#include <stdio.h>
#include <stddef.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "clist.h"
#include "tokenize.h"
#include "token.h"
#include <stddef.h>


// Documented in .h file
const char *TT_to_str(TokenType tt)
{
  switch(tt) {
  case TOK_VALUE:
    return "VALUE";
  case TOK_PLUS:
    return "PLUS";
  case TOK_MINUS:
    return "MINUS";
  case TOK_MULTIPLY:
    return "MULTIPLY";
  case TOK_DIVIDE:
    return "DIVIDE";
  case TOK_POWER:
    return "POWER";
  case TOK_OPEN_PAREN:
    return "OPEN_PAREN";
  case TOK_CLOSE_PAREN:
    return "CLOSE_PAREN";
  case TOK_END:
    return "(end)";
  }

  __builtin_unreachable();
}


// Documented in .h file
CList TOK_tokenize_input(const char *input, char *errmsg, size_t errmsg_sz) {
    CList tokens = CL_new();
    size_t i = 0;

    while (input[i] != '\0') {
        if (isspace(input[i])) {
            i++; // Skip whitespace
            continue;
        }

        Token token;
        
        // Handle numbers (integers and doubles)
        if (isdigit(input[i]) || input[i] == '.') {
            char *endptr;
            double value = strtod(&input[i], &endptr);

            if (endptr == &input[i]) {
                snprintf(errmsg, errmsg_sz, "Position %zu: Illegal numeric value", i + 1);
                CL_free(tokens);
                return NULL;
            }

            // Check if we've consumed any characters for the number
            i += (endptr - &input[i]);
            token.type = TOK_VALUE;
            token.value = value;
        } else {
            // Handle operators and parentheses
            switch (input[i]) {
                case '+': token.type = TOK_PLUS; break;
                case '-': token.type = TOK_MINUS; break;
                case '*': token.type = TOK_MULTIPLY; break;
                case '/': token.type = TOK_DIVIDE; break;
                case '^': token.type = TOK_POWER; break;
                case '(': token.type = TOK_OPEN_PAREN; break;
                case ')': token.type = TOK_CLOSE_PAREN; break;
                default:
                    // Handle unexpected characters with specific position
                    snprintf(errmsg, errmsg_sz, "Position %zu: unexpected character %c", i + 1, input[i]);
                    CL_free(tokens);
                    return NULL;
            }
            token.value = 0; // Operators and parentheses don't have a value
            i++;
        }

        // Add token to the list
        CL_append(tokens, token);
    }
    
    // Add end-of-input token
    Token end_token = { .type = TOK_END, .value = 0 };
    CL_append(tokens, end_token);

    return tokens;
}



// Documented in .h file
TokenType TOK_next_type(CList tokens) {
    if (CL_length(tokens) == 0) {
        return TOK_END;
    }
    Token token = CL_nth(tokens, 0);
    return token.type;
}

// Documented in .h file
Token TOK_next(CList tokens) {
    return CL_nth(tokens, 0);
}



// Documented in .h file
void TOK_consume(CList tokens) {
    if (CL_length(tokens) > 0) {
        CL_pop(tokens);
    }
}


void printToken(int pos, CListElementType element, void* cb_data) {
  if(element.type == TOK_VALUE) {
    // TODO: print token type and value
    printf("Position %d: Token type: VALUE, Value: %g\n", pos, TT_to_str(element.value));
  } else {
    // TODO: print token type
    printf("Position %d: Token type: %d\n", pos, TT_to_str(element.type));
  }
}

// Documented in .h file
void TOK_print(CList tokens) {
    CL_foreach(tokens, printToken, NULL);
}
