#include <stdio.h>
#include <string.h>

#include "tokenize.h"

void test_tokenize(const char *input, const char *expected_output) {
    char errmsg[256];
    CList tokens = TOK_tokenize_input(input, errmsg, sizeof(errmsg));

    if (tokens == NULL) {
        printf("Error: %s\n", errmsg);
        return;
    }

int main() {
    test_tokenize("1 + 2", "TOK_VALUE(1) TOK_PLUS TOK_VALUE(2) TOK_END");
    test_tokenize("(1 + 2) * 3", "TOK_OPEN_PAREN TOK_VALUE(1) TOK_PLUS TOK_VALUE(2) TOK_CLOSE_PAREN TOK_MULTIPLY TOK_VALUE(3) TOK_END");
    // Add more tests as needed
    return 0;
}
