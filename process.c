#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "process.h"
#include "token.h"
#include "data_set.h"

VAR_LIST* var_list = NULL;

#define PROC_AST_NODE(N, out, GOTO)   if(!(out = process_ast(ast->node[N]))){goto GOTO;}
#define RUNTIME_ERR(GOTO, fmt, ...)   {printf("runtime error: "); \
                                printf(fmt, ##__VA_ARGS__); \
                                printf("\n"); \
                                goto GOTO;}

AST* process_add(AST* ast)
{
    AST *left = NULL;
    AST *right = NULL;
    AST *out = NULL;
    char *buffer;
    int est_len=0;
    PROC_AST_NODE(0, left, _ADD_END);
    PROC_AST_NODE(1, right, _ADD_END);

    switch (left->type)
    {
    case STR:
        switch (right->type)
        {
        case INT:
            est_len = snprintf(NULL, 0, "%s%d", left->data.s, right->data.i);
            buffer = (char*)malloc(sizeof(char)*est_len + 1);
            memset(buffer, 0, est_len);
            sprintf(buffer, "%s%d", left->data.s, right->data.i);
            est_len = strlen(buffer);
            out = ast_new(STR, buffer, est_len);
            free(buffer);
            break;
        case REAL:
            est_len = snprintf(NULL, 0, "%s%lf", left->data.s, right->data.r);
            buffer = (char*)malloc(sizeof(char)*est_len + 1);
            memset(buffer, 0, est_len);
            
            sprintf(buffer, "%s%lf", left->data.s, right->data.r);
            est_len = strlen(buffer);
            out = ast_new(STR, buffer, est_len);
            
            free(buffer);
            break;
        case STR:
            est_len = snprintf(NULL, 0, "%s%s", left->data.s, right->data.s);
            buffer = (char*)malloc(sizeof(char)*est_len + 1);
            memset(buffer, 0, est_len);
            
            sprintf(buffer, "%s%s", left->data.s, right->data.s);
            est_len = strlen(buffer);
            out = ast_new(STR, buffer, est_len);
            
            free(buffer);
            break;
        }
        break;
    case INT:
        switch (right->type)
        {
        case INT:
            out = ast_clone(left);
            out->data.i += right->data.i;
            break;
        case REAL:
            out = ast_clone(right);
            out->data.r += left->data.i*1.0;
            break;
        case STR:
            est_len = snprintf(NULL, 0, "%d%s", left->data.i, right->data.s);
            buffer = (char*)malloc(sizeof(char)*est_len + 1);
            memset(buffer, 0, est_len);
            sprintf(buffer, "%d%s", left->data.i, right->data.s);
            est_len = strlen(buffer);
            out = ast_new(STR, buffer, est_len);
            free(buffer);
            break;
        }
        break;
    case REAL:
        switch (right->type)
        {
        case INT:
            out = ast_clone(left);
            out->data.r += right->data.i*1.0;
            break;
        case REAL:
            out = ast_clone(left);
            out->data.r += right->data.r;
            break;
        case STR:
            est_len = snprintf(NULL, 0, "%lf%s", left->data.r, right->data.s);
            buffer = (char*)malloc(sizeof(char)*est_len + 1);
            memset(buffer, 0, est_len);

            sprintf(buffer, "%lf%s", left->data.r, right->data.s);
            est_len = strlen(buffer);
            out = ast_new(STR, buffer, est_len);

            free(buffer);
            break;
        }
        break;
    }
       

_ADD_END:
    if(left)
        ast_free_all(left);
    if(right)
        ast_free_all(right);
    return out;
}

AST* process_sub(AST* ast)
{
    AST *left = NULL;
    AST *right = NULL;
    AST *out = NULL;
    double tmp;
    PROC_AST_NODE(0, left, _SUB_END);
    PROC_AST_NODE(1, right, _SUB_END);

    switch (left->type)
    {
    case INT:
        switch (right->type)
        {
        case INT:
            out = ast_clone(left);
            out->data.i -= right->data.i;
            break;
        case REAL:
            out = ast_clone(right);
            tmp = left->data.i*1.0 - out->data.r;
            out->data.r = tmp;
            break;
        default:
            RUNTIME_ERR(_SUB_END, "%s - %s 는 정의되지 않음",
                        type_l[left->type & TOKEN_MASK],
                        type_l[right->type & TOKEN_MASK]);
        }
        break;
    case REAL:
        switch (right->type)
        {
        case INT:
            out = ast_clone(left);
            out->data.r -= right->data.i*1.0;
            break;
        case REAL:
            out = ast_clone(left);
            out->data.r -= right->data.r;
            break;
        default:
            RUNTIME_ERR(_SUB_END, "%s - %s 는 정의되지 않음",
                        type_l[left->type & TOKEN_MASK],
                        type_l[right->type & TOKEN_MASK]);
        }
        break;
    default:
        RUNTIME_ERR(_SUB_END, "%s - %s 는 정의되지 않음",
                        type_l[left->type & TOKEN_MASK],
                        type_l[right->type & TOKEN_MASK]);
        break;
    }
       

_SUB_END:
    if(left)
        ast_free_all(left);
    if(right)
        ast_free_all(right);
    return out;
}

AST* process_mul(AST* ast)
{
    AST *left = NULL;
    AST *right = NULL;
    AST *out = NULL;
    char* buffer = NULL;
    int est_len=0;
    PROC_AST_NODE(0, left, _MUL_END);
    PROC_AST_NODE(1, right, _MUL_END);

    switch (left->type)
    {
    case STR:
        switch (right->type)
        {
        case INT:
            est_len = left->data_len * right->data.i;
            buffer = (char*)malloc(sizeof(char)*est_len + 1);
            memset(buffer, 0, est_len);
            for(int i=0; i<right->data.i; i++)
                memcpy(&buffer[i*left->data_len],left->data.s, left->data_len);
            out = ast_new(STR, buffer, est_len);
            free(buffer);
            break;
        default:
            RUNTIME_ERR(_MUL_END, "%s * %s 는 정의되지 않음",
                        type_l[left->type & TOKEN_MASK],
                        type_l[right->type & TOKEN_MASK]);
        }
        break;
    case INT:
        switch (right->type)
        {
        case INT:
            out = ast_clone(left);
            out->data.i *= right->data.i;
            break;
        case REAL:
            out = ast_clone(right);
            out->data.r *= left->data.i*1.0;
            break;
        default:
            RUNTIME_ERR(_MUL_END, "%s * %s 는 정의되지 않음",
                        type_l[left->type & TOKEN_MASK],
                        type_l[right->type & TOKEN_MASK]);
        }
        break;
    case REAL:
        switch (right->type)
        {
        case INT:
            out = ast_clone(left);
            out->data.r *= right->data.i*1.0;
            break;
        case REAL:
            out = ast_clone(left);
            out->data.r *= right->data.r;
            break;
        default:
            RUNTIME_ERR(_MUL_END, "%s * %s 는 정의되지 않음",
                        type_l[left->type & TOKEN_MASK],
                        type_l[right->type & TOKEN_MASK]);
        }
        break;
    }
       
_MUL_END:
    if(left)
        ast_free_all(left);
    if(right)
        ast_free_all(right);
    return out;
}

AST* process_div(AST* ast)
{
    AST *left = NULL;
    AST *right = NULL;
    AST *out = NULL;
    double tmp;
    int loc, count;
    PROC_AST_NODE(0, left, _DIV_END);
    PROC_AST_NODE(1, right, _DIV_END);

    switch (left->type)
    {
    case STR:
        switch (right->type)
        {
        case STR:
            if(right->data_len == 0)
                RUNTIME_ERR(_DIV_END, "우측 문자열이 비어 있음");
            count = 0;
            loc = 0;
            while(loc < left->data_len)
            {
                if(strncmp(&left->data.s[loc], right->data.s, right->data_len))
                    break;
                count++;
                loc += right->data_len;
            }
            out = ast_new(INT, &count, 0);
            break;
        default:
            RUNTIME_ERR(_DIV_END, "%s / %s 는 정의되지 않음",
                        type_l[left->type & TOKEN_MASK],
                        type_l[right->type & TOKEN_MASK]);
        }
        break;
    case INT:
        switch (right->type)
        {
        case INT:
            if(right->data.i == 0)
                RUNTIME_ERR(_DIV_END, "0 으로 나눔");
            out = ast_clone(left);
            out->data.i /= right->data.i;
            break;
        case REAL:
            if(right->data.r == 0.0)
                RUNTIME_ERR(_DIV_END, "0 으로 나눔");
            out = ast_clone(right);
            tmp = left->data.i*1.0 / out->data.r;
            out->data.r = tmp;
            break;
        default:
            RUNTIME_ERR(_DIV_END, "%s / %s 는 정의되지 않음",
                        type_l[left->type & TOKEN_MASK],
                        type_l[right->type & TOKEN_MASK]);
        }
        break;
    case REAL:
        switch (right->type)
        {
        case INT:
            if(right->data.i == 0)
                RUNTIME_ERR(_DIV_END, "0 으로 나눔");
            out = ast_clone(left);
            out->data.r /= right->data.i*1.0;
            break;
        case REAL:
            if(right->data.r == 0.0)
                RUNTIME_ERR(_DIV_END, "0 으로 나눔");
            out = ast_clone(left);
            out->data.r /= right->data.r;
            break;
        default:
            RUNTIME_ERR(_DIV_END, "%s / %s 는 정의되지 않음",
                        type_l[left->type & TOKEN_MASK],
                        type_l[right->type & TOKEN_MASK]);
        }
        break;
    }
       
_DIV_END:
    if(left)
        ast_free_all(left);
    if(right)
        ast_free_all(right);
    return out;
}

AST* process_sub_func(AST* ast)
{
    AST* string = NULL;
    AST *num1 = NULL;
    AST *num2 = NULL;
    AST *out = NULL;
    int n1, n2;
    PROC_AST_NODE(0, string, _SUB_FUNC_END);
    PROC_AST_NODE(1, num1, _SUB_FUNC_END);
    PROC_AST_NODE(2, num2, _SUB_FUNC_END);

    if(string->type != STR
    || num1->type != INT
    || num2->type != INT)
        RUNTIME_ERR(_SUB_FUNC_END, "sub(string, int, int) 형식이 아닌 sub(%s, %s, %s)\n", 
            type_l[string->type & TOKEN_MASK],
            type_l[num1->type & TOKEN_MASK],
            type_l[num2->type & TOKEN_MASK]);

    if((n1 = num1->data.i) < 0)
        RUNTIME_ERR(_SUB_FUNC_END, "n1 값이 음수");
    if((n2 = num2->data.i) < 0)
        RUNTIME_ERR(_SUB_FUNC_END, "n1 값이 음수");
    
    if(string->data_len < n1 + n2)
        RUNTIME_ERR(_SUB_FUNC_END, "string < num1 + num2");
        
    char* new_string = (char*)malloc(sizeof(char)*n2 + 1);
    memset(new_string, 0, sizeof(char)*n2 + 1);
    memcpy(new_string, &string->data.s[n1], n2);
    out = ast_new(STR, new_string, n2);

    free(new_string);

_SUB_FUNC_END:
    if(string)
        ast_free_all(string);
    if(num1)
        ast_free_all(num1);
    if(num2)
        ast_free_all(num2);
    return out;

}

AST* process_assign(AST* ast)
{
    AST* data = NULL;
    PROC_AST_NODE(1, data, _ASSIGN_END);
    
    vl_set(var_list, ast->node[0]->data.s, data);

_ASSIGN_END:
    return data;
}

AST* process_unary(AST* ast)
{
    AST* data = NULL;
    AST* out = NULL;
    PROC_AST_NODE(0, data, _UNARY_END);
    
    switch (data->type)
    {
    case INT:
        out = ast_clone(data);
        out->data.i *= ast->data.i;
        break;
    case REAL:
        out = ast_clone(data);
        out->data.r *= (double)ast->data.i;
        break;
    default:
        RUNTIME_ERR(_UNARY_END, "unary * %s 는 정의되지 않음", type_l[data->type & TOKEN_MASK]);
        break;
    }
_UNARY_END:
    if(data)
        ast_free_all(data);
    return out;
}

AST* process_var(AST* ast)
{
    AST* out = NULL;
    if(!(out = vl_get(var_list, ast->data.s)))
        RUNTIME_ERR(_VAR_END, "%s는 undefined variable",ast->data.s);
_VAR_END:
    return out;
}

AST* process_ast(AST* ast)
{
    if(ast == NULL)
        return NULL;
    if(ast->type == VAR)
        return process_var(ast);
    if(!(ast->type & OPERATOR))
        return ast_clone(ast);
    switch (ast->type)
    {
    case ADD:
        return process_add(ast);
    case SUB:
        return process_sub(ast);
    case MUL:
        return process_mul(ast);
    case DIV:
        return process_div(ast);
    case SUB_FUNC:
        return process_sub_func(ast);
    case ASSIGN:
        return process_assign(ast);
    case UNARY:
        return process_unary(ast);
    default:
        return NULL;
    }
}