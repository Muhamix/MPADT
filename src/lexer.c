#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../include/lexer.h"
static const char *input;
static size_t pos;
void init_lexer(const char *source) {
    input = source;
    pos = 0;
}
static void skip_whitespace() {while (isspace(input[pos])) pos++;}
static Token make_token(tokentype type, const char *start, size_t length) {
    Token t;
    t.type = type;
    t.lexeme = malloc(length + 1);
    strncpy(t.lexeme, start, length);
    t.lexeme[length] = '\0';
    return t;
}
Token get_next_token() {
    skip_whitespace();
    if (input[pos] == '\0') return make_token(TOKEN_EOF, "", 0);
    char c = input[pos];
    switch (c) {
        case '{':
            pos++;
            return make_token(TOKEN_LBRACE, "{", 1);
        case '}':
            pos++;
            return make_token(TOKEN_RBRACE, "}", 1);
        case ',':
            pos++;
            return make_token(TOKEN_COMMA, ",", 1);
        case '-':
            if (input[pos + 1] == '>') {
                pos += 2;
                return make_token(TOKEN_ARROW, "->", 2);
            }
        case '"':
            pos++;
            size_t start = pos;
            while (input[pos] && input[pos] != '"') pos++;
            if (input[pos] != '"') {
                fprintf(stderr, "Unterminated string literal\n");
                exit(1);
            }
            size_t length = pos - start;
            pos++;
            return make_token(TOKEN_STRING, input + start, length);
        default:
            if (isalpha(c)) {
                size_t start = pos;
                while (isalnum(input[pos]) || input[pos] == '_') pos++;
                size_t length = pos - start;
                char* lex = malloc(length + 1);
                strncpy(lex, input + start, length);
                lex[length] = '\0';
            Token t = make_token(TOKEN_IDENTIFIER, lex, length);
            free(lex);
            return t;
        }
    }
    pos++;
    return make_token(TOKEN_UNKNOWN, &c, 1);
}
void free_token(Token token) {free(token.lexeme);}

