#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "data_set.h"
#include "token.h"
#include "utils.h"

STACK* stack_init(void)
{
    STACK* stack = (STACK*)malloc(sizeof(STACK));
    memset(stack, 0, sizeof(STACK));
    stack->point = 0;
    stack->len = 256;
    return stack;
}

void stack_push(STACK* stack, int var)
{
    if(stack->point + 1 == stack->len)
        ERROR("stack overflow");
    
    stack->stack[stack->point] = var;
    stack->point++;
}

int stack_pop(STACK* stack)
{
    if(stack->point == -1)
        ERROR("stack underflow");
    
    stack->point--;
    return stack->stack[stack->point];
}

void stack_free(STACK* stack)
{
    free(stack);
}

PARSING_TABLE* pt_build(void)
{
    PARSING_TABLE* pt;
    pt = (PARSING_TABLE*)malloc(sizeof(PARSING_TABLE));
    memset(pt, 0, sizeof(PARSING_TABLE));

    // node zero is error
    pt->node_len++;

    pt->node[pt->node_len++] = ptn_list(1, NT_ASSIGN);

    pt->node[pt->node_len++] = ptn_list(2, VAR, NT_ASSIGNd);
    pt->node[pt->node_len++] = ptn_list(4, NT_ADDOP, NT_FACTOR_VAR, NT_EXP_MULd, NT_EXP_ADDd);
    pt->node[pt->node_len++] = ptn_list(3, NT_FACTOR_ETC, NT_EXP_MULd, NT_EXP_ADDd);

    pt->node[pt->node_len++] = ptn_list(2, ASSIGN, NT_ASSIGN);
    pt->node[pt->node_len++] = ptn_list(2, NT_EXP_MULd, NT_EXP_ADDd);

    pt->node[pt->node_len++] = ptn_list(2, NT_EXP_MUL, NT_EXP_ADDd);
    pt->node[pt->node_len++] = ptn_list(3, NT_ADDOP, NT_EXP_MUL, NT_EXP_ADDd);
    pt->node[pt->node_len++] = ptn_list(0);
    
    pt->node[pt->node_len++] = ptn_list(2, NT_EXP_UNARY, NT_EXP_MULd);
    pt->node[pt->node_len++] = ptn_list(3, NT_MULOP, NT_EXP_UNARY, NT_EXP_MULd);
    pt->node[pt->node_len++] = ptn_list(0);
    
    pt->node[pt->node_len++] = ptn_list(2, NT_ADDOP, NT_FACTOR_VAR);
    pt->node[pt->node_len++] = ptn_list(1, NT_FACTOR_VAR);

    pt->node[pt->node_len++] = ptn_list(1, VAR);
    pt->node[pt->node_len++] = ptn_list(1, NT_FACTOR_ETC);

    pt->node[pt->node_len++] = ptn_list(3, LPAREN, NT_ASSIGN, RPAREN);
    pt->node[pt->node_len++] = ptn_list(7, SUB_FUNC, NT_EXP_ADD, COMMA, NT_EXP_ADD, COMMA, NT_EXP_ADD, RPAREN);
    pt->node[pt->node_len++] = ptn_list(1, INT);
    pt->node[pt->node_len++] = ptn_list(1, REAL);
    pt->node[pt->node_len++] = ptn_list(1, STR);

    pt->node[pt->node_len++] = ptn_list(1, ADD);
    pt->node[pt->node_len++] = ptn_list(1, SUB);

    pt->node[pt->node_len++] = ptn_list(1, MUL);
    pt->node[pt->node_len++] = ptn_list(1, DIV);
    int parsing_table_ori[13][14] = {
//               $   V   S   I   R   +   -   *   /   (   ) sub   ,   =
              {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
/*input     */{  0,  1,  1,  1,  1,  1,  1,  0,  0,  1,  0,  1,  0,  0},
/*assign    */{  0,  2,  4,  4,  4,  3,  3,  0,  0,  4,  0,  4,  0,  0},
/*assign'   */{  6,  0,  0,  0,  0,  6,  6,  6,  6,  0,  6,  0,  0,  5},
/*exp_add   */{  0,  7,  7,  7,  7,  7,  7,  0,  0,  7,  0,  7,  0,  0},
/*exp_add'  */{  9,  0,  0,  0,  0,  8,  8,  0,  0,  0,  9,  0,  9,  0},
/*exp_mul   */{  0, 10, 10, 10, 10, 10, 10,  0,  0, 10,  0, 10,  0,  0},
/*exp_mul'  */{ 12,  0,  0,  0,  0, 12, 12, 11, 11,  0, 12,  0, 12,  0},
/*exp_unary */{  0, 14, 14, 14, 14, 13, 13,  0,  0, 14,  0, 14,  0,  0},
/*factor_var*/{  0, 15, 16, 16, 16,  0,  0,  0,  0, 16,  0, 16,  0,  0},
/*factor_etc*/{  0,  0, 21, 19, 20,  0,  0,  0,  0, 17,  0, 18,  0,  0},
/*addop     */{  0,  0,  0,  0,  0, 22, 23,  0,  0,  0,  0,  0,  0,  0},
/*mulop     */{  0,  0,  0,  0,  0,  0,  0, 24, 25,  0,  0,  0,  0,  0},
    };
    for(int i=0;i<13;i++)
    {
        for(int j=0;j<14;j++)
            pt->table[i][j] = parsing_table_ori[i][j];
    }
    return pt;
}


PARSING_TABLE_NODE* ptn_list(int num, ...)
{
    va_list args;
    PARSING_TABLE_NODE* node;
    
    node = (PARSING_TABLE_NODE*)malloc(sizeof(PARSING_TABLE_NODE));
    memset(node, 0, sizeof(node));

    node->len = num;

    va_start(args, num);
    for(int i=0; i<num; i++)
        node->list[i] = va_arg(args, int);
    
    va_end(args);

    return node;
}

void pt_release(PARSING_TABLE* pt)
{
    for(int i=1; i<pt->node_len; i++)
        free(pt->node[i]);
    free(pt);
}