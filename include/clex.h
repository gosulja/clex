#ifndef CLEXLIB_H
#define CLEXLIB_H

#define CLEX_VERSION 1

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

char* strndup(const char* str, size_t len);

typedef enum TokenType {
    Identifier,     // Variable/function names, etc.
    Number,         // Numeric literals (e.g., integers, floats)
    String,         // String literals (e.g., "hello")

    Whitespace,     // Spaces, tabs, newlines
    Comment,        // Single-line or multi-line comments

    LParen,         // (
    RParen,         // )
    LBrace,         // {
    RBrace,         // }
    LBracket,       // [
    RBracket,       // ]
    Comma,          // ,
    Semicolon,      // ;
    Colon,          // :
    Period,         // .
    Arrow,          // ->
    Ellipsis,       // ...

    Add,            // +
    Sub,            // -
    Mul,            // *
    Div,            // /
    Mod,            // %
    Power,          // **
    Equals,         // =
    PlusEquals,     // +=
    MinusEquals,    // -=
    MulEquals,      // *=
    DivEquals,      // /=
    ModEquals,      // %=
    Increment,      // ++
    Decrement,      // --

    LT,             // <
    GT,             // >
    LTE,            // <=
    GTE,            // >=
    EQ,             // ==
    NEQ,            // !=

    And,            // &&
    Or,             // ||
    Not,            // !
    BitwiseAnd,     // &
    BitwiseOr,      // |
    BitwiseXor,     // ^
    BitwiseNot,     // ~
    ShiftLeft,      // <<
    ShiftRight,     // >>
    LogicalNullish, // ??

    If,             // if
    Else,           // else
    While,          // while
    For,            // for
    Do,             // do
    Switch,         // switch
    Case,           // case
    Default,        // default
    Break,          // break
    Continue,       // continue
    Return,         // return
    Function,       // function or def
    Class,          // class
    Struct,         // struct
    Enum,           // enum
    Import,         // import
    Export,         // export
    Try,            // try
    Catch,          // catch
    Finally,        // finally
    Throw,          // throw
    Async,          // async
    Await,          // await
    New,            // new
    Delete,         // delete
    This,           // this or self
    Super,          // super
    Null,           // null or nil
    True,           // true
    False,          // false
    Undefined,      // undefined
    Yield,          // yield

    Hash,           // #
    Dollar,         // $
    At,             // @
    Backslash,      /* \ */
    QuestionMark,   // ?
    ExclamationMark,// !
    Pipe,           // |
    Ampersand,      // &
    Asterisk,       // *
    Underscore,     // _
    Backtick,       // `
    Tilde,          // ~

    T_EOF,            // End of file / input
} TokenType;

typedef struct Token {
    TokenType   type;
    const char* value;
} Token;

typedef struct Lexer {
    const char* source;
    size_t      read;
    size_t      position;
    char        current;
} Lexer;

int test(int a, int b);

void init_lexer(Lexer* lexer, const char* source);
void destroy_lexer(Lexer* lexer);
void advance(Lexer* lexer);

bool is_alpha(char c);
bool is_digit(char c);

Token next(Lexer* lexer);

#if defined(CLEX_IMPLEMENTATION)
    char* strndup(const char* str, size_t len) {
        char* dup = (char*)malloc(len + 1);
        if (dup) {
            memcpy(dup, str, len);
            dup[len] = '\0';
        }
        return dup;
    }

    void init_lexer(Lexer* lexer, const char* source) {
        lexer->source = source;
        lexer->read = 0;
        lexer->position = 0;
        lexer->current = source[0];
    }

    void destroy_lexer(Lexer* lexer) {
        printf("clex <destroy_lexer>: todo\n");
    }

    Token next(Lexer* lexer) {
        while (lexer->current == ' ' || lexer->current == '\t' || lexer->current == '\n' || lexer->current == '\r') {
            advance(lexer);
        }

        if (lexer->current == '\0')
            return (Token){T_EOF, NULL};

        /*
            Identifier literals.

            If the first character is an alpahabetical character e.g 'a'
            then we can advance, and check if it's an alpha and a digit for identifiers such as:
            hello2, bye4, etc.
        */
        if (is_alpha(lexer->current)) {
            size_t start = lexer->position;
            while (is_alpha(lexer->current) || is_digit(lexer->current)) {
                advance(lexer);
            }

            size_t len = lexer->position - start;
            char* value = strndup(lexer->source + start, len);
            
            return (Token){Identifier, value};
        }

        /*
            Number literals.

            If the first character is a digit, and then encounters a period, the is_float flag will be set to true to signify
            it is a float, and the first and only decimal point has been passed.
        */
        if (is_digit(lexer->current)) {
            size_t start = lexer->position;
            while (is_digit(lexer->current)) {
                advance(lexer);
            }

            size_t len = lexer->position - start;
            char* value = strndup(lexer->source + start, len);
            
            return (Token){Number, value};
        }

        switch (lexer->current) {
            case '(': advance(lexer); return (Token){LParen, "("};
            case ')': advance(lexer); return (Token){RParen, ")"};
            case '{': advance(lexer); return (Token){LBrace, "{"};
            case '}': advance(lexer); return (Token){RBrace, "}"};
            case '+': advance(lexer); return (Token){Add, "+"};
            case '-': advance(lexer); return (Token){Sub, "-"};
            case '*': advance(lexer); return (Token){Mul, "*"};
            case '/': advance(lexer); return (Token){Div, "/"};
            case '%': advance(lexer); return (Token){Mod, "%"};
        }

        /* Unrecognised characters. */
        char* value = strndup(&lexer->current, 1);
        advance(lexer);
        return (Token){Undefined, value};
    }

    void advance(Lexer* lexer) {
        if (lexer->current != '\0') {
            lexer->read++;
            lexer->position++;
            lexer->current = lexer->source[lexer->read];
        }
    }

    bool is_digit(char c) {
        return c >= '0' && c <= '9';
    }

    bool is_alpha(char c) {
        return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
    }

    int test(int a, int b) {
        return a + b;
    }
#endif

#endif