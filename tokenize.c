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

        if (isdigit(input[i])) {
            // Parse number
            int value = 0;
            while (isdigit(input[i])) {
                value = value * 10 + (input[i] - '0');
                i++;
            }
            token.type = TOK_VALUE;
            token.value = value;
        } else {
            // Parse operators and parentheses
            switch (input[i]) {
                case '+': token.type = TOK_PLUS; break;
                case '-': token.type = TOK_MINUS; break;
                case '*': token.type = TOK_MULTIPLY; break;
                case '/': token.type = TOK_DIVIDE; break;
                case '^': token.type = TOK_POWER; break;
                case '(': token.type = TOK_OPEN_PAREN; break;
                case ')': token.type = TOK_CLOSE_PAREN; break;
                default:
                    snprintf(errmsg, errmsg_sz, "Invalid character: %c", input[i]);
                    CL_free(tokens);
                    return NULL;
            }
            token.value = 0;  // Operators don't have a value
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
    Token *token = (Token *)CL_head(tokens);
    return token->type;
}

// Documented in .h file
Token TOK_next(CList tokens) {
    if (CL_length(tokens) == 0) {
        Token end_token = { .type = TOK_END, .value = 0 };
        return end_token;
    }
    return *(Token *)CL_head(tokens);
}



// Documented in .h file
void TOK_consume(CList tokens) {
    if (CL_length(tokens) > 0) {
        CL_pop(tokens);
    }
}




// Documented in .h file
void TOK_print(CList tokens) {
    CL_foreach(tokens, token) {
        Token *t = (Token *)token;
        printf("%s", TT_to_str(t->type));
        if (t->type == TOK_VALUE) {
            printf("(%d)", t->value);
        }
        printf(" ");
    }
    printf("\n");
}
