#ifndef CLEXLIB_H
#define CLEXLIB_H

#define CLEX_VERSION 1.1

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct CLexConfig {
    struct {
        bool enable_single_line_comments;
        bool enable_multi_line_comments;
        const char* single_line_comment_start[4];
        const char* multi_line_comment_start;
        const char* multi_line_comment_end;
    } comments;

    struct {
        bool ignore_whitespaces;
        bool case_sensitive_keywords;
    } lexer;
} CLexConfig;

typedef enum CLexTokenType {
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
    Function,       // function, func, fn, or def
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
    Pipe,           // |
    Ampersand,      // &

    T_EOF,            // End of file / input
} CLexTokenType;

typedef struct CLexToken {
    CLexTokenType   type;
    const char* value;
} CLexToken;

typedef struct CLex {
    const char* source;
    size_t      read;
    size_t      position;
    char        current;
} CLex;

typedef struct CLexKeywordEntry {
    const char* name;
    CLexTokenType type;
} CLexKeywordEntry;

int test(int a, int b);

void cl_set_config(CLex* lexer, CLexConfig* cfg);

void init_clex(CLex* lexer, const char* source);
void dest_clex(CLex* lexer);
void clex_advance(CLex* lexer);

bool cl_is_alpha(char c);
bool cl_is_digit(char c);
bool cl_is_hex_digit(char c);

char cl_peek(CLex* lexer);
char cl_peek2(CLex* lexer);

CLexToken cl_next(CLex* lexer);

static const CLexKeywordEntry keyword_table[] = {
        {"async", Async},
        {"await", Await},
        {"break", Break},
        {"case", Case},
        {"catch", Catch},
        {"class", Class},
        {"continue", Continue},
        {"def", Function},
        {"default", Default},
        {"delete", Delete},
        {"do", Do},
        {"else", Else},
        {"enum", Enum},
        {"export", Export},
        {"finally", Finally},
        {"fn", Function},
        {"for", For},
        {"func", Function},
        {"function", Function},
        {"if", If},
        {"import", Import},
        {"new", New},
        {"nil", Null},
        {"null", Null},
        {"return", Return},
        {"self", This},
        {"struct", Struct},
        {"super", Super},
        {"switch", Switch},
        {"throw", Throw},
        {"this", This},
        {"true", True},
        {"try", Try},
        {"while", While},
        {"yield", Yield}
};

CLexTokenType cl_find_keyword(const char* str);

#if defined(CLEX_IMPLEMENTATION)
    CLexTokenType cl_find_keyword(const char* str) {
        size_t left = 0;
        size_t right = sizeof(keyword_table) / sizeof(keyword_table[0]) - 1;

        while (left <= right) {
            size_t mid = left + (right - left) / 2;
            int cmp = strcmp(str, keyword_table[mid].name);

            if (cmp == 0) {
                return keyword_table[mid].type;
            }

            if (cmp < 0) {
                right = mid - 1;
            } else {
                left = mid + 1;
            }
        }

        return Identifier;
    }

    char* parse_string_literal(CLex* lexer, char quote_char) {
        size_t start = lexer->position + 1;
        size_t max_len = strlen(lexer->source + start);
        char* buffer = malloc(max_len + 1);
        size_t buf_index = 0;
        bool escaped = false;

        clex_advance(lexer);

        while (lexer->current != '\0') {
            if (!escaped && lexer->current == quote_char) {
                break;
            }

            if (escaped) {
                switch (lexer->current) {
                    case 'n': buffer[buf_index++] = '\n'; break;
                    case 'r': buffer[buf_index++] = '\r'; break;
                    case 't': buffer[buf_index++] = '\t'; break;
                    case '\\': buffer[buf_index++] = '\\'; break;
                    case '"': buffer[buf_index++] = '"'; break;
                    case '\'': buffer[buf_index++] = '\''; break;
                    case '0': buffer[buf_index++] = '\0'; break;
                    case 'a': buffer[buf_index++] = '\a'; break;
                    case 'b': buffer[buf_index++] = '\b'; break;
                    case 'f': buffer[buf_index++] = '\f'; break;
                    case 'v': buffer[buf_index++] = '\v'; break;
                    case 'x': {
                        char hex[3] = {0};
                        if (cl_is_hex_digit(cl_peek(lexer))) {
                            hex[0] = lexer->current;
                            clex_advance(lexer);
                            if (cl_is_hex_digit(cl_peek(lexer))) {
                                hex[1] = lexer->current;
                                clex_advance(lexer);
                            }
                            buffer[buf_index++] = (char)strtol(hex, NULL, 16);
                        } else {
                            buffer[buf_index++] = '\\';
                            buffer[buf_index++] = 'x';
                        }
                        break;
                    }
                    default:
                        buffer[buf_index++] = '\\';
                        buffer[buf_index++] = lexer->current;
                        break;
                }
                escaped = false;
            }
            else if (lexer->current == '\\') {
                escaped = true;
            }
            else {
                buffer[buf_index++] = lexer->current;
            }

            clex_advance(lexer);
        }

        buffer[buf_index] = '\0';

        if (lexer->current == quote_char) {
            clex_advance(lexer);
        }

        return buffer;
    }

    bool cl_is_hex_digit(char c) {
        return (c >= '0' && c <= '9') ||
               (c >= 'a' && c <= 'f') ||
               (c >= 'A' && c <= 'F');
    }

    void cl_set_config(CLex* lexer, CLexConfig* cfg) {
        if (cfg->comments.enable_single_line_comments) {
            for (int i = 0; i < 4; i++) {
                if (cfg->comments.single_line_comment_start[i] == NULL) break;

                if (strlen(cfg->comments.single_line_comment_start[i]) == 0 ||
                    strlen(cfg->comments.single_line_comment_start[i]) > 3) {
                    fprintf(stderr, "clex: invalid single line comment starter!\n");
                    return;
                }
            }
        }

        if (cfg->comments.enable_multi_line_comments) {
            if (!cfg->comments.multi_line_comment_start ||
                !cfg->comments.multi_line_comment_end ||
                strlen(cfg->comments.multi_line_comment_start) == 0 ||
                strlen(cfg->comments.multi_line_comment_end) == 0) {
                fprintf(stderr, "clex: invalid multi line comment configuration!\n");
                return;
            }
        }

        // todo: implement real configuration and usage in clex
    }

    void init_clex(CLex* lexer, const char* source) {
        lexer->source = source;
        lexer->read = 0;
        lexer->position = 0;
        lexer->current = source[0];
    }

    void dest_clex(CLex* lexer) {
        printf("clex <dest_clex>: todo\n");
    }

    CLexToken cl_next(CLex* lexer) {
        while (lexer->current == ' ' || lexer->current == '\t' || lexer->current == '\n' || lexer->current == '\r') {
            clex_advance(lexer);
        }

        if (lexer->current == '\0')
            return (CLexToken){T_EOF, NULL};

        /*
            Identifier literals.

            If the first character is an alphabetical character e.g 'a'
            then we can clex_advance, and check if it's an alpha and a digit for identifiers such as:
            hello2, bye4, etc.
        */
        if (cl_is_alpha(lexer->current)) {
            const char* start = lexer->source + lexer->position;
            while (cl_is_alpha(lexer->current) || cl_is_digit(lexer->current)) {
                clex_advance(lexer);
            }

            char* value = strdup(start);
            char* end = value + (lexer->position - (start - lexer->source));
            *end = '\0';

            CLexTokenType type = cl_find_keyword(value);
            if (type != Identifier) {
                return (CLexToken){type, value};
            }

            return (CLexToken){Identifier, value};
        }

        /*
            Number literals.

            If the first character is a digit, and then encounters a period, the is_float flag will be set to true to signify
            it is a float, and the first and only decimal point has been passed.
        */
        if (cl_is_digit(lexer->current)) {
            const char* start = lexer->source + lexer->position;
            while (cl_is_digit(lexer->current)) {
                clex_advance(lexer);
            }

            char* value = strdup(start);
            char* end = value + (lexer->position - (start - lexer->source));
            *end = '\0';

            return (CLexToken){Number, value};
        }

        /*
            String literals.

            If the first character is a double quote or a single quote, it'll accept any character.
            Includes full character escape functionality, handled by the parse_string_literal function.
        */
        if (lexer->current == '"' || lexer->current == '\'') {
            char quote_char = lexer->current;
            char* value = parse_string_literal(lexer, quote_char);

            return (CLexToken){String, value};
        }

        switch (lexer->current) {
            case '(':
                clex_advance(lexer); return (CLexToken){LParen, "("};
            case ')':
                clex_advance(lexer); return (CLexToken){RParen, ")"};
            case '{':
                clex_advance(lexer); return (CLexToken){LBrace, "{"};
            case '}':
                clex_advance(lexer); return (CLexToken){RBrace, "}"};
            case '[':
                clex_advance(lexer); return (CLexToken){LBracket, "["};
            case ']':
                clex_advance(lexer); return (CLexToken){RBracket, "]"};
            case ':':
                clex_advance(lexer); return (CLexToken){Colon, ":"};
            case '.': {
                if (cl_peek(lexer) == '.' && cl_peek2(lexer) == '.') {
                    clex_advance(lexer);
                    clex_advance(lexer);
                    clex_advance(lexer);
                    return (CLexToken){Ellipsis, "..."};
                }

                clex_advance(lexer);
                return (CLexToken){Period, "."};
            }

            case ';':
                clex_advance(lexer); return (CLexToken){Semicolon, ";"};
            case ',':
                clex_advance(lexer); return (CLexToken){Comma, ","};
            case '+': {
                if (cl_peek(lexer) == '+') {
                    clex_advance(lexer);
                    clex_advance(lexer);
                    return (CLexToken) { Increment, "++" };
                } else if (cl_peek(lexer) == '=') {
                    clex_advance(lexer);
                    clex_advance(lexer);
                    return (CLexToken) { PlusEquals, "+=" };
                }

                clex_advance(lexer);
                return (CLexToken) {Add, "+"};
            }
            case '-':
                if (cl_peek(lexer) == '>') {
                    clex_advance(lexer);
                    clex_advance(lexer);
                    return (CLexToken) {Arrow, "->"};
                } else if (cl_peek(lexer) == '=') {
                    clex_advance(lexer);
                    clex_advance(lexer);
                    return (CLexToken) { MinusEquals, "-=" };
                } else if (cl_peek(lexer) == '-') {
                    clex_advance(lexer);
                    clex_advance(lexer);
                    return (CLexToken) { Decrement, "--" };
                }

                clex_advance(lexer);
                return (CLexToken){Sub, "-"};
            case '*':
                if (cl_peek(lexer) == '*') {
                    clex_advance(lexer);
                    return (CLexToken){Power, "**"};
                } else if (cl_peek(lexer) == '=') {
                    clex_advance(lexer);
                    clex_advance(lexer);
                    return (CLexToken) { MulEquals, "*=" };
                }

                clex_advance(lexer);
                return (CLexToken){Mul, "*"};
            case '/':
                if (cl_peek(lexer) == '=') {
                    clex_advance(lexer);
                    clex_advance(lexer);
                    return (CLexToken) { DivEquals, "/=" };
                }

                clex_advance(lexer);
                return (CLexToken){Div, "/"};
            case '%':
                if (cl_peek(lexer) == '=') {
                    clex_advance(lexer);
                    clex_advance(lexer);
                    return (CLexToken) { ModEquals, "%=" };
                }

                clex_advance(lexer);
                return (CLexToken){Mod, "%"};
            case '=':
                if (cl_peek(lexer) == '=') {
                    clex_advance(lexer);
                    clex_advance(lexer);
                    return (CLexToken) { EQ, "==" };
                }

                clex_advance(lexer);
                return (CLexToken){Equals, "="};
            case '<':
                if (cl_peek(lexer) == '=') {
                    clex_advance(lexer);
                    clex_advance(lexer);
                    return (CLexToken) { LTE, "<=" };
                } else if (cl_peek(lexer) == '<') {
                    clex_advance(lexer);
                    clex_advance(lexer);
                    return (CLexToken) { ShiftLeft, "<<" };
                }

                clex_advance(lexer);
                return (CLexToken){LT, "<"};
            case '>':
                if (cl_peek(lexer) == '=') {
                    clex_advance(lexer);
                    clex_advance(lexer);
                    return (CLexToken) { GTE, ">=" };
                } else if (cl_peek(lexer) == '>') {
                    clex_advance(lexer);
                    clex_advance(lexer);
                    return (CLexToken) { ShiftRight, ">>" };
                }

                clex_advance(lexer);
                return (CLexToken){GT, ">"};
            case '?':
                if (cl_peek(lexer) == '?') {
                    clex_advance(lexer);
                    clex_advance(lexer);
                    return (CLexToken) { LogicalNullish, "??" };
                }

                return (CLexToken) { QuestionMark, "?" };
            case '!':
                if (cl_peek(lexer) == '=') {
                    clex_advance(lexer);
                    clex_advance(lexer);
                    return (CLexToken) { NEQ, "!=" };
                }

                clex_advance(lexer);
                return (CLexToken){Not, "!"};
            case '&':
                if (cl_peek(lexer) == '&') {
                    clex_advance(lexer);
                    clex_advance(lexer);
                    return (CLexToken) { And, "&&" };
                }

                clex_advance(lexer);
                return (CLexToken) { Ampersand, "&" };
            case '|':
                if (cl_peek(lexer) == '|') {
                    clex_advance(lexer);
                    clex_advance(lexer);
                    return (CLexToken) { Or, "||" };
                }

                return (CLexToken) { Pipe, "|" };
            case '^': clex_advance(lexer); return (CLexToken) { BitwiseXor, "^" };
            case '~': clex_advance(lexer); return (CLexToken) { BitwiseNot, "~" };
            case '#': clex_advance(lexer); return (CLexToken) { Hash, "#" };
            case '$': clex_advance(lexer); return (CLexToken) { Dollar, "$" };
            case '@': clex_advance(lexer); return (CLexToken) { At, "@" };
            case '\\': clex_advance(lexer); return (CLexToken) { Backslash, "\\" };
            default:
                break;
        }

        /* Unrecognised characters. */
        if (lexer->current != '\0') {
            char temp[2] = {lexer->current, '\0'};
            char* value = strdup(temp);
            clex_advance(lexer);
            return (CLexToken){Undefined, value};
        }

        return (CLexToken){T_EOF, NULL};
    }

    char cl_peek(CLex* lexer) {
        if (lexer->read+1 != '\0') {
            return lexer->source[lexer->read+1];
        }

        return '\0';
    }

    char cl_peek2(CLex* lexer) {
        if (lexer->read+2 != '\0') {
            return lexer->source[lexer->read+2];
        }

        return '\0';
    }

    void clex_advance(CLex* lexer) {
        if (lexer->current != '\0') {
            lexer->read++;
            lexer->position++;
            lexer->current = lexer->source[lexer->read];
        }
    }

    bool cl_is_digit(char c) {
        return c >= '0' && c <= '9';
    }

    bool cl_is_alpha(char c) {
        return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
    }

    int test(int a, int b) {
        return a + b;
    }
#endif

#endif