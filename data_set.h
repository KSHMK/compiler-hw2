#ifndef __DATA_SET_H__
#define __DATA_SET_H__

typedef struct abstract_syntax_tree {
    int type;
    int data_len;
    union {
        char* s;
        int i;
        double f;
        char c;
    } data;

    struct abstract_syntax_tree* node[3];
    
} AST;

AST* ast_new(int type, void* data, int data_len);
void ast_add_node(AST* ast, int i, AST* node);
void ast_free_all(AST* ast);
void ast_print(AST* ast);

#endif
