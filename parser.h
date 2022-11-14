#ifndef __PARSER_H__
#define __PARSER_H__

#include "data_set.h"

AST* input(AST* in);
AST* assign(AST* in);
AST* assignd(AST* in);
AST* exp_add(AST* in);
AST* exp_addd(AST* in);
AST* exp_mul(AST* in);
AST* exp_muld(AST* in);
AST* exp_unary(AST* in);
AST* factor_var(AST* in);
AST* factor_etc(AST* in);
AST* addop(AST* in);
AST* mulop(AST* in);
AST* unaryop(AST* in);

AST* process_add(AST* ast);
AST* process_sub(AST* ast);
AST* process_mul(AST* ast);
AST* process_div(AST* ast);
AST* process_sub_func(AST* ast);
AST* process_assign(AST* ast);
AST* process_unary(AST* ast);
AST* process_var(AST* ast);
AST* process_ast(AST* ast);

AST* parser(AST* in);
AST* parse_input(TOKEN* token_root);

TOKEN* lexing_input(char* input_buffer);

#endif