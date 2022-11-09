#ifndef __DATA_SET_H__
#define __DATA_SET_H__

typedef struct abstract_syntax_tree {
    int type;
    int data_len;
    union {
        char* v,s;
        int i;
        double f;
    } data;
    
} AST;



#endif
