#ifndef __DATA_SET_H__
#define __DATA_SET_H__

typedef struct parsing_table_node {
    int len;
    int list[10];
} PARSING_TABLE_NODE;

typedef struct parsing_table {
    int node_len;
    PARSING_TABLE_NODE* node[30]; 
    int table[13][14];
} PARSING_TABLE;

typedef struct stack {
    int point;
    int len;
    int stack[256];
} STACK;

STACK* stack_init(void);
void stack_push(STACK* stack, int var);
int stack_pop(STACK* stack);
void stack_free(STACK* stack);

PARSING_TABLE* pt_build(void);
PARSING_TABLE_NODE* ptn_list(int num, ...);
void pt_release(PARSING_TABLE* pt);

#endif
