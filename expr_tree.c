/*
 * expr_tree.c
 * 
 * A dynamically allocated tree to handle arbitrary arithmetic
 * expressions
 *
 * Author: <Pauline Uwase>
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include "expr_tree.h"

#define LEFT 0
#define RIGHT 1

struct _expr_tree_node {
  ExprNodeType type;
  union {
    struct _expr_tree_node *child[2];
    double value;
  } n;
};


/*
 * Convert an ExprNodeType into a printable character
 *
 * Parameters:
 *   ent    The ExprNodeType to convert
 * 
 * Returns: A single character representing the ent
 */
static char ExprNodeType_to_char(ExprNodeType ent) {
    switch (ent) {
        case OP_ADD: return '+';
        case OP_SUB: return '-';
        case OP_MUL: return '*';
        case OP_DIV: return '/';
        case OP_POWER: return '^';
        case UNARY_NEGATE: return '-';
        default: return '?';
    }
}



// Documented in .h file
ExprTree ET_value(double value) {
    ExprTree new = (ExprTree)malloc(sizeof(struct _expr_tree_node));
    assert(new);
    new->type = VALUE;
    new->n.value = value;
    return new;
}



// Documented in .h file
ExprTree ET_node(ExprNodeType op, ExprTree left, ExprTree right) {
    ExprTree new = (ExprTree)malloc(sizeof(struct _expr_tree_node));
    assert(new);
    new->type = op;
    new->n.child[LEFT] = left;
    new->n.child[RIGHT] = right;
    return new;
}



// Documented in .h file
void ET_free(ExprTree tree) {
    if (tree == NULL) return;

    if (tree->type != VALUE) {
        ET_free(tree->n.child[LEFT]);
        ET_free(tree->n.child[RIGHT]);
    }

    free(tree);
}



// Documented in .h file
int ET_count(ExprTree tree)
{
  if (tree == NULL)
    return 0;

  if (tree->type == VALUE)
    return 1;

  return 1 + ET_count(tree->n.child[LEFT]) + ET_count(tree->n.child[RIGHT]);
}


// Documented in .h file
int ET_depth(ExprTree tree) {
    if (tree == NULL) return 0;
    if (tree->type == VALUE) return 1;

    int left_depth = ET_depth(tree->n.child[LEFT]);
    int right_depth = ET_depth(tree->n.child[RIGHT]);
    return 1 + (left_depth > right_depth ? left_depth : right_depth);
}



// Documented in .h file
double ET_evaluate(ExprTree tree) {
    //assert(tree);
    if (tree == NULL) return 0;
    
    if (tree->type == VALUE) return tree->n.value;

    double left_val = ET_evaluate(tree->n.child[LEFT]);
    double right_val = ET_evaluate(tree->n.child[RIGHT]);

    switch (tree->type) {
        case OP_ADD: return left_val + right_val;
        case OP_SUB: return left_val - right_val;
        case OP_MUL: return left_val * right_val;
        case OP_DIV: return left_val / right_val;
        case OP_POWER: return pow(left_val, right_val);
        case UNARY_NEGATE: return -left_val;
        default: return 0.0;
    }
}



// Documented in .h file
size_t ET_tree2string(ExprTree tree, char *buf, size_t buf_sz) {
    if (buf_sz == 0 || tree == NULL) return 0;

    // Handle VALUE nodes
    if (tree->type == VALUE) {
        return snprintf(buf, buf_sz, "%g", tree->n.value);
    }

    // Handle UNARY NEGATE type specifically
    if (tree->type == UNARY_NEGATE) {
        size_t len = 0;
        if (buf_sz > 1) {
            buf[len++] = '(';
        }

        // Add the unary minus operator
        if (len < buf_sz - 1) {
            buf[len++] = ExprNodeType_to_char(UNARY_NEGATE);
        }

        size_t child_len = ET_tree2string(tree->n.child[LEFT], buf + len, buf_sz - len);
        len += child_len;

        // Check if the buffer overflowed
        if (len >= buf_sz) {
            buf[buf_sz - 1] = '$';
            buf[buf_sz - 2] = '\0';
            return buf_sz - 1; 
        }

        // Add closing parenthesis
        if (len < buf_sz - 1) {
            buf[len++] = ')';
        }

        buf[len] = '\0';
        return len;
    }

    char op = ExprNodeType_to_char(tree->type);

    size_t len = 0;
    if (buf_sz > 1) {
        buf[len++] = '(';
    }

    size_t left_len = ET_tree2string(tree->n.child[LEFT], buf + len, buf_sz - len);
    len += left_len;
    if (len >= buf_sz - 1) return len;

    if (len < buf_sz - 1) {
        buf[len++] = op;
    }

    size_t right_len = ET_tree2string(tree->n.child[RIGHT], buf + len, buf_sz - len);
    len += right_len;
    if (len >= buf_sz - 1) return len;

    if (len < buf_sz - 1) {
        buf[len++] = ')';
    }

    buf[len] = '\0';

    if (len >= buf_sz) {
        buf[buf_sz - 1] = '$';
        buf[buf_sz - 2] = '\0';
        return buf_sz - 1;
    }

    return len;
}

