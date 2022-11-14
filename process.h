#ifndef __PROCESS_H__
#define __PROCESS_H__

#include "data_set.h"

extern VAR_LIST* var_list;

AST* process_add(AST* ast);
AST* process_sub(AST* ast);
AST* process_mul(AST* ast);
AST* process_div(AST* ast);
AST* process_sub_func(AST* ast);
AST* process_assign(AST* ast);
AST* process_unary(AST* ast);
AST* process_var(AST* ast);

AST* process_ast(AST* ast);

#endif