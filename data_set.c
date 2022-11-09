#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "data_set.h"
#include "token.h"
#include "utils.h"

AST* ast_new(int type, void* data, int data_len)
{
    AST* ast;
    char* str;

    ast = (AST*)malloc(sizeof(AST));
    memset(ast, 0, sizeof(AST));

    ast->type = type;
    ast->data_len = data_len;
    switch (type)
    {
    case VAR:
    case STR:
        str = (char*)malloc(data_len+1);
        memset(str, 0, data_len+1);
        memcpy(str, data, data_len);
        ast->data.s = str;
        break;
    case INT:
    case UNARY:
        ast->data.i = *(int*)data;
        break;
    case REAL:
        ast->data.f = *(double*)data;
        break;
    default:
        ast->data.c = (char)data;
        break;
    }
}
void ast_add_node(AST* ast, int i, AST* node)
{
    ast->node[i] = node;
}

void ast_free_all(AST* ast)
{
    if(!ast)
        return;

    if(ast->type == VAR
    || ast->type == STR)
        free(ast->data.s);
    for(int i=0; i<3; i++)
        ast_free_all(ast->node[i]);
    free(ast);
}

void ast_print_data(AST* ast)
{
    switch (ast->type)
    {
    case VAR:
    case STR:
        printf("%s ",ast->data.s);
        break;
    case INT:
    case UNARY:
        printf("%d ",ast->data.i);
        break;
    case REAL:
        printf("%lf ",ast->data.f);
        break;
    case SUB_FUNC:
        printf("sub3 ");
        break;
    default:
        printf("%c2 ",ast->data.c);
        break;
    }
}

void ast_print_node(AST* ast)
{
    int c = 0;
    for(int i=0; i<3; i++)
    {
        if(ast->node[i])
        {
            ast_print_data(ast->node[i]);
            c++;
        }
    }
    if(c != 0)
        printf("\n");
    for(int i=0; i<3; i++)
    {
        if(ast->node[i])
            ast_print_node(ast->node[i]);
    }
    
}

void ast_print(AST* ast)
{
    ast_print_data(ast);
    if(ast->node[0])
        printf("\n");
    ast_print_node(ast);
    
       
}