#ifndef LEXER_H
#define LEXER_H
typedef enum {
    TOKEN_EOF,
    TOKEN_IDENTIFIER,
    TOKEN_LBRACE,
    TOKEN_RBRACE,
    TOKEN_ARROW,
    TOKEN_STRING,
    TOKEN_COMMA,
    TOKEN_UNKNOWN
} TokenType;
typedef struct {
    TokenType type;
    char *lexeme;
} Token;
void init_lexer(const char *source);
Token get_next_token();
void free_token(Token token);
#endif
