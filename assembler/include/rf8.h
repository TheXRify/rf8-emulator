#ifndef RF8_H
#define RF8_H

// token types
// STRING
// CHARACTER
// INTEGER
typedef enum tokentype {
    STRING,
    CHARACTER,
    INTEGER,
    INSTRUCTION,
    COMMA,
    NEWLINE,
    LPAREN, RPAREN,
    COMMENT,
    BQUOTE, EQUOTE,
    BDBLQUOTE, EDBLQUOTE,
    REGISTER
} tokentype_t;

typedef struct token {
    tokentype_t type;
    union {
        char* str;
        int integer;
        char chr;
    } value;
} token_t;

#endif // RF8_H