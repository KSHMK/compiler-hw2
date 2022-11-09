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

AST* parser(AST* in);
void process_input(char* input_buffer);

#endif