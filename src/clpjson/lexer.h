#pragma once

#include <stdlib.h>

#define MAX_STR_VALUE_SIZE 256

#define nullptr ((void*)0)

typedef enum TokenKind {
    T_INVALID,
    T_EOF,

    T_LBRACE,
    T_RBRACE,
    T_LSQUARE,
    T_RSQAURE,

    T_COLON,
    T_COMMA,

    T_INTLIT,
    T_STRLIT,
} TokenKind;

typedef struct Token {
    TokenKind k;
    union {
        int b;
        int i;
        float f;
        char s[MAX_STR_VALUE_SIZE];
    } v;
} Token;

int scan(Token* t);
