# clex

A header-only, universal tokenization library for C.

# current token support
* Identifier
* Number
* LParen
* RParen
* LBRace
* RBrace
* Add
* Sub
* Mul
* Div
* Mod
* Undefined

# usage
Place `clex.h` somewhere in your project, make sure it's included.

```
#define CLEX_IMPLEMENTATION
#include "clex.h"

...
// Usage example.
const char* test_src = "1+2";

Lexer lexer;
init_lexer(&lexer);

Token tok = clex_next(&lexer);
while (tok.type != T_EOF) {
    prinft("type: %d, value: %s\n", tok.type, tok.value);
    tok = clex_next(&lexer);
}

destroy_lexer(&lexer);  // Not implemented until further development.
```