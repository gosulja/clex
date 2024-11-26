#define CLEX_IMPLEMENTATION
#include "../include/clex.h"

#include <assert.h>
#include <stdio.h>

int main() {
    assert(test(5, 5) == 10);
    printf("Successfully passed test.\n");

    const char* test_src = "1+2";

    Lexer lexer;
    init_lexer(&lexer, test_src);

    printf("Created lexer with test source: %s\n", test_src);

    Token tok = next(&lexer);
    while (tok.type != T_EOF) {
        printf("Type: %d, Value: %s\n", tok.type, tok.value);
        tok = next(&lexer);
    }

    destroy_lexer(&lexer);

    return 0;
}