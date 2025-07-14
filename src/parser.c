#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/parser.h"
static ArchRule arch_rules[10];
static int arch_rule_count = 0;
static Token current_token;
extern Token get_next_token();
static const char *input;
static size_t pos = 0;
void set_input(const char *src) {
    input = src;
    pos = 0;
}
extern void free_token(Token token);
static void error(const char *msg) {
    fprintf(stderr, "Parse error: %s\n", msg);
    exit(1);
}
static void consume(tokentype type) {
    if (current_token.type != type) error("Unexpected token");
    free_token(current_token);
    current_token = get_next_token();
}
static char *expect_identifier() {
    if (current_token.type != TOKEN_IDENTIFIER) error("Expected identifier");
    char *id = strdup(current_token.lexeme);
    free_token(current_token);
    current_token = get_next_token();
    return id;
}
static void expect(tokentype type) {
    if (current_token.type != type) error("Unexpected token");
    free_token(current_token);
    current_token = get_next_token();
}
static void parse_rule() {
    char *arch = expect_identifier();
    expect(TOKEN_ARROW);
    if (current_token.type != TOKEN_STRING) error("Expected a string");
    char *filename = strdup(current_token.lexeme);
    free_token(current_token);
    current_token = get_next_token(); 
    arch_rules[arch_rule_count].arch_name = arch;
    arch_rules[arch_rule_count].files.file = filename;
    arch_rule_count++;
}
void parse_program() {
    current_token = get_next_token();
    if (current_token.type != TOKEN_IDENTIFIER || strcmp(current_token.lexeme, "arch") != 0) error("Expected 'arch' keyword");
    free_token(current_token);
    current_token = get_next_token();
    expect(TOKEN_LBRACE);
    while (current_token.type == TOKEN_IDENTIFIER) parse_rule();
    expect(TOKEN_RBRACE);
}
ArchRule *get_arch_rules(int *count) {
    *count = arch_rule_count;
    return arch_rules;
}

