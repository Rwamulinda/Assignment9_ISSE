#include <stdio.h>
#include "tokenize.h"

void test_tokenize(const char *input, const char *expected_output) {
    char errmsg[256];
    CList tokens = TOK_tokenize_input(input, errmsg, sizeof(errmsg));

    if (tokens == NULL) {
        printf("Error: %s\n", errmsg);
        return;
    }

    // Assuming you have a way to convert tokens to a string for comparison
    char *output = tokens_to_string(tokens); // Implement this function to format the output
    if (strcmp(output, expected_output) == 0) {
        printf("Test passed for input: \"%s\"\n", input);
    } else {
        printf("Test failed for input: \"%s\". Expected: \"%s\", got: \"%s\"\n", input, expected_output, output);
    }

    // Clean up
    CL_free(tokens);
    free(output); // If tokens_to_string allocates memory
}

int main() {
    test_tokenize("1 + 2", "TOK_VALUE(1) TOK_PLUS TOK_VALUE(2) TOK_END");
    test_tokenize("(1 + 2) * 3", "TOK_OPEN_PAREN TOK_VALUE(1) TOK_PLUS TOK_VALUE(2) TOK_CLOSE_PAREN TOK_MULTIPLY TOK_VALUE(3) TOK_END");
    // Add more tests as needed
    return 0;
}
