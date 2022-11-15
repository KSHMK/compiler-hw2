#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "data_set.h"
#include "token.h"

char* type_l[] = {
    "$",
    "variable",
    "string",
    "int",
    "real",
    "+",
    "-",
    "*",
    "/",
    "(",
    ")",
    "sub(",
    ",",
    "=",
    "unary",
    "ERR"
};

TOKEN* tl_new(int type, char* text, int len, TOKEN* prev)
{
    TOKEN* token = (TOKEN*)malloc(sizeof(TOKEN));
    memset(token, 0, sizeof(TOKEN));
    token->type = type;
    token->len = len;
    token->text = (char*)malloc(sizeof(char)*len + 1);
    memset(token->text, 0, sizeof(char)*len + 1);
    memcpy(token->text, text, len);
    if(prev)
        prev->next = token;
    return token;
}

void tl_free(TOKEN *token)
{
    TOKEN* next;
    while(token)
    {
        next = token->next;
        free(token->text);
        free(token);
        token = next;
    }
}

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
        if(data_len > 10)
            data_len = 10;
        str = (char*)malloc(data_len+1);
        memset(str, 0, data_len+1);
        memcpy(str, data, data_len);
        ast->data.s = str;
        break;
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
        ast->data.r = *(double*)data;
        break;
    default:
        ast->data.c = *(char*)&data;
        break;
    }
    return ast;
}

AST* ast_clone(AST* ast)
{
    AST* ast_new;
    char* str;

    ast_new = (AST*)malloc(sizeof(AST));
    memset(ast_new, 0, sizeof(AST));
    ast_new->type = ast->type;
    ast_new->data_len = ast->data_len;
    switch (ast->type)
    {
    case VAR:
    case STR:
        str = (char*)malloc(ast->data_len+1);
        memset(str, 0, ast->data_len+1);
        memcpy(str, ast->data.s, ast->data_len);
        ast_new->data.s = str;
        break;
    case INT:
    case UNARY:
        ast_new->data.i = ast->data.i;
        break;
    case REAL:
        ast_new->data.r = ast->data.r;
        break;
    default:
        ast_new->data.c = ast->data.c;
        break;
    }
    return ast_new;
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
        printf("%s ",ast->data.s);
        break;
    case STR:
        printf("\"%s\" ",ast->data.s);
        break;
    case INT:
    case UNARY:
        printf("%d ",ast->data.i);
        break;
    case REAL:
        printf("%lf ",ast->data.r);
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
    if(!ast)
        return;
        
    ast_print_data(ast);
    printf("\n");
    ast_print_node(ast);      
}

VAR_LIST* vl_init(void)
{
    VAR_LIST* vl = (VAR_LIST*)malloc(sizeof(VAR_LIST));
    memset(vl, 0, sizeof(VAR_LIST));
    return vl;
}

void vl_set(VAR_LIST* vl, char* name, AST* data_ori)
{
    int cmp_len;
    AST* data = ast_clone(data_ori);
    cmp_len = strlen(name);
    if(cmp_len > 10)
        cmp_len = 10;
    
    // check var is exist
    for(int i=0; i<vl->len; i++)
    {
        if(!strncmp(vl->name[i], name, cmp_len))
        {
            ast_free_all(vl->data[i]);
            vl->data[i] = data;
            return;
        }
    }

    memcpy(vl->name[vl->len], name, cmp_len + 1);
    vl->data[vl->len] = data;
    vl->len++;
    
}
AST* vl_get(VAR_LIST* vl, char* name)
{
    int cmp_len;
    AST* ast;
    cmp_len = strlen(name);
    if(cmp_len > 10)
        cmp_len = 10;
    
    // check var is exist
    for(int i=0; i<vl->len; i++)
    {
        if(!strncmp(vl->name[i], name, cmp_len))
        {
            ast = vl->data[i];
            return ast_clone(ast);
        }
    }
    return NULL;
}

void vl_free(VAR_LIST* vl)
{
    for(int i=0; i<vl->len; i++)
        ast_free_all(vl->data[i]);
    free(vl);
}

void vl_print(VAR_LIST* vl)
{
    printf("name\t\ttype\t\tvalue\n");
    for(int i=0; i<vl->len; i++)
    {
        printf("%s\t\t",vl->name[i]);
        printf("%s\t\t",type_l[vl->data[i]->type & TOKEN_MASK]);
        switch(vl->data[i]->type) {
        case INT:
            printf("%d\n",vl->data[i]->data.i);
            break;
        case REAL:
            printf("%lf\n",vl->data[i]->data.r);
            break;
        case STR:
            printf("\"%s\"\n",vl->data[i]->data.s);
            break;
        }
    }
}

