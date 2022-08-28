// Copyright (C) 2022-2023 56software
// SPDX-License-Identifier: GPL-3.0

#include "lexer.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>

static int isopen = 0;
static FILE* infile;
static int _putback = 0;

// Get the next character from the input file.
static int next(void) {
    if(!isopen) {
        infile = fopen("./clpac.json", "r");
        if(!infile) {
            printf("Failed to open \"clpac.json\" file");
            exit(0);
        }
        isopen = 1;
    }

    int c;

    if(_putback) {
        c = _putback;
        _putback = 0;
        return c;
    }

    c = fgetc(infile);
    return c;
}

// Skip past input that we don't need to deal with, 
// i.e. whitespace, newlines. Return the first
// character we do need to deal with.
static int skip(void) {
    int c;

    c = next();
    while(c == ' ' || c == '\n' || c == '\r' || c == '\t' || c == '\f') {
        c = next();
    }

    return c;
}

// Put back an unwanted character
static void putback(int c) { _putback = c; }

// Return the position of character c
// in string s, or -1 if c not found
static int chrpos(char *s, int c) {
  char *p;

  p = strchr(s, c);
  return (p ? p - s : -1);
}

// Scan and return an integer literal
// value from the input file. Store
// the value as a string in Text.
static int scanint(int c) {
    int k, val = 0;

    // Convert each character into an int value
    while ((k = chrpos("0123456789", c)) >= 0) {
        val = val * 10 + k;
        c = next();
    }

    // We hit a non-integer character, put it back.
    putback(c);
    return val;
}

// Return the next character from a character
// or string literal
static int scanch(void) {
  int c;

  // Get the next input character and interpret
  // metacharacters that start with a backslash
  c = next();
  if (c == '\\') {
    switch (c = next()) {
      case 'a':  return '\a';
      case 'b':  return '\b';
      case 'f':  return '\f';
      case 'n':  return '\n';
      case 'r':  return '\r';
      case 't':  return '\t';
      case 'v':  return '\v';
      case '\\': return '\\';
      case '"':  return '"' ;
      case '\'': return '\'';
      default:
        printf("unknown escape sequence\n");
    }
  }
  return (c);                   // Just an ordinary old character!
}

// Scan in a string literal from the input file,
// and store it in buf[]. Return the length of
// the string. 
static int scanstr(char* buf) {
    int i, c;

    for(i = 0; i < MAX_STR_VALUE_SIZE - 1; i++) {
        if((c = scanch()) == '"') {
            buf[i] = 0;
            return i;
        }
        buf[i] = c;
    }

    printf("Str literal too long\n");
    return 0;
}

// Scan and return the next token found in the input.
// Return 1 if token valid, 0 if no tokens left.
int scan(Token *t) {
    int c;

    // Skip whitespace
    c = skip();
    switch(c) {
        case EOF:
            t->k = T_EOF;
            return 0;

        // Skipping comments
        case '/':
            if((c = next()) == '/') {
                while(c != '\n' || c != EOF) {
                    c = next();
                }

                if(c == EOF) {
                    t->k = T_EOF;
                    return 0;
                }

                return scan(t);
            } else if(c == '*') {
                while(c != EOF) {
                    if(c == '*') {
                        if((c = next()) == '/')
                            break;
                        else
                            putback(c);
                    }
                    c = next();
                }

                if(c == EOF) {
                    t->k = T_EOF;
                    return 0;
                }

                return scan(t);
            } else {
                putback(c);
            }

        case '{':
            t->k = T_LBRACE;
            break;
        
        case '}':
            t->k = T_RBRACE;
            break;

        case '[':
            t->k = T_LSQUARE;
            break;

        case ']':
            t->k = T_RSQAURE;
            break;

        case '"':
            scanstr(t->v.s);
            t->k = T_STRLIT;
            break;

        case ':':
            t->k = T_COLON;
            break;

        case ',':
            t->k = T_COMMA;
            break;
        
        default:
            // No float literal support
            if(isdigit(c)) {
                t->k = T_INTLIT;
                t->v.i = scanint(c);
                break;
            }
        
            // TODO: Error handling
            printf("error\n");
            break;
    }

    return 1;
}
