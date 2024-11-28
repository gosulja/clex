# clex

A header-only, universal tokenization library for C, up to 83 of the most common tokens and keywords found across programming languages.

# configuration
If you want to include your own custom keywords into the lexer, simply find the `keyword_table`, add your entry, and create a new token type for your keyword.

# current token support
- [x] Identifier
- [x] Number
- [x] String
- [ ] Whitespace
- [ ] Comment
- [x] LParen
- [x] RParen
- [x] LBrace
- [x] RBrace
- [x] LBracket
- [x] RBracket
- [x] Comma
- [x] Semicolon
- [x] Colon
- [x] Period
- [x] Arrow
- [x] Ellipsis
- [x] Add
- [x] Sub
- [x] Mul
- [x] Div
- [x] Mod
- [x] Power
- [x] Equals
- [x] PlusEquals
- [x] MinusEquals
- [x] MulEquals
- [x] DivEquals
- [x] ModEquals
- [x] Increment
- [x] Decrement
- [x] LT
- [x] GT
- [x] LTE
- [x] GTE
- [x] EQ
- [x] NEQ
- [x] And
- [x] Or
- [x] Not
- [x] BitwiseXor
- [x] BitwiseNot
- [x] ShiftLeft
- [x] ShiftRight
- [x] LogicalNullish
- [x] If
- [x] Else
- [x] While
- [x] For
- [x] Do
- [x] Switch
- [x] Case
- [x] Default
- [x] Break
- [x] Continue
- [x] Return
- [x] Function
- [x] Class
- [x] Struct
- [x] Enum
- [x] Import
- [x] Export
- [x] Try
- [x] Catch
- [x] Finally
- [x] Throw
- [x] Async
- [x] Await
- [x] New
- [x] Delete
- [x] This
- [x] Super
- [x] Null
- [x] True
- [x] False
- [x] Undefined
- [x] Yield
- [x] Hash
- [x] Dollar
- [x] At
- [x] Backslash
- [x] QuestionMark
- [x] Pipe
- [x] Ampersand
- [x] T_EOF

# usage
Place `clex.h` somewhere in your project, make sure it's included.

```c
#define CLEX_IMPLEMENTATION
#include "clex.h"

...
// Usage example.
CLex clex;
init_clex(&clex, "{}()[]<>...->$@*^");

CLexToken tok = cl_next(&clex);
while (tok.type != T_EOF) {
    printf("token { type: %d, value: %s }\n", tok.type, tok.value);
    tok = cl_next(&clex);
}
```

Output
```
token { type: 7, value: { }
token { type: 8, value: } }
token { type: 5, value: ( }
token { type: 6, value: ) }
token { type: 9, value: [ }
token { type: 10, value: ] }
token { type: 31, value: < }
token { type: 32, value: > }
token { type: 16, value: ... }
token { type: 15, value: -> }
token { type: 78, value: $ }
token { type: 79, value: @ }
token { type: 19, value: * }
token { type: 40, value: ^ }
```