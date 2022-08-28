
#include "clpjson/lexer.h"
#include <stdio.h>

const char *tok_str[] = {"INVALID", "EOF", "{", "}", "[", "]", ":", ",", "Int", "Str"};

int main(int argc, char** argv) {
    Token t;
    while(scan(&t)) {
        printf("Token: %s", tok_str[t.k]);
        if(t.k == T_INTLIT)
            printf("\tValue: %d", t.v.i);
        else if(t.k == T_STRLIT)
            printf("\tValue: %s", t.v.s);
        printf("\n");
    }
    printf("Token: EOF\n");
    return 0;
}
