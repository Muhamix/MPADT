#ifndef PARSER_H
#define PARSER_H
#include "lexer.h"
typedef struct {
    char *file;
} RunFiles;
typedef struct {
    char *arch_name;
    RunFiles files;
} ArchRule;
void parse_program();
ArchRule *get_arch_rules(int *count);
void set_input(const char *src);
#endif
