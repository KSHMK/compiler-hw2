#ifndef __DATA_SET_H__
#define __DATA_SET_H__

typedef struct token_node {
    int type;
    int len;
    char* text;
    struct token_node* next;
} TOKEN;

typedef struct abstract_syntax_tree {
    int type;
    int data_len;
    union ast_data {
        char* s;
        int i;
        double r;
        char c;
    } data;

    struct abstract_syntax_tree* node[3];
    
} AST;

typedef struct var_list {
    int len;
    char name[100][11];
    AST* data[100];
} VAR_LIST;

extern char* type_l[];

TOKEN* tl_new(int type, char* text, int len, TOKEN* prev);
void tl_free(TOKEN *token);

AST* ast_new(int type, void* data, int data_len);
AST* ast_clone(AST* ast);
void ast_add_node(AST* ast, int i, AST* node);
void ast_free_all(AST* ast);
void ast_print(AST* ast);

VAR_LIST* vl_init(void);
void vl_set(VAR_LIST* vl, char* name, AST* data);
AST* vl_get(VAR_LIST* vl, char* name);
void vl_free(VAR_LIST* vl);
void vl_print(VAR_LIST* vl);

#endif
