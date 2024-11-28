#define CLEX_IMPLEMENTATION
#include "../include/clex.h"

#include <assert.h>
#include <stdio.h>

int main(void) {
    printf("Running CLex tests...\n\n");

    CLex clex;
    init_clex(&clex, "{}()[]<>...->$@*^");

    CLexToken tok = cl_next(&clex);
    while (tok.type != T_EOF) {
        printf("token { type: %d, value: %s }\n", tok.type, tok.value);
        tok = cl_next(&clex);
    }

    printf("\nTests completed.\n");
    return 0;
}