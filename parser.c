#include <stdio.h>
#include "parser.h"
#include "token.h"
#include "data_set.h"

extern char *yytext;
extern int yyleng;
extern int yylex();

extern void* yy_scan_string( const char *yy_str  );
extern void yy_switch_to_buffer ( void* new_buffer  );
extern void yy_delete_buffer ( void* b  );

char* tok_l[] = {
    "$",
    "VAR",
    "STR",
    "INT",
    "REAL",
    "ADD",
    "SUB",
    "MUL",
    "DIV",
    "LPAREN",
    "RPAREN",
    "SUB_FUNC",
    "COMMA",
    "ASSIGN",
};

char* expect_l[] = {
    "",
    "variable",
    "string",
    "int",
    "real",
    "",
    "",
    "",
    "",
    "(",
    ")",
    "sub(",
    ",",
    "=",
};
#define SKIP                ((AST*)1)
#define DORET(func)         DO(func, ret)
#define DO(func, out)       if(!(out=(func)(in))){return 0;}
#define DOEPS(func, inp)    {in = inp; DORET(func); if(ret == SKIP){ret = in;}}
#define next_tok()          if(!next_tok_raw()){return 0;}
int tok;
int line_loc;

int next_tok_raw(void)
{
    line_loc += yyleng;
    tok = yylex();
    if(tok == ERR)
    {
        printf("lexical error(%d): %s\n",line_loc, yytext);
        return 0;
    }
    return 1;
}


int expect(int tok_check)
{
    if(tok == tok_check){
        return next_tok_raw();
    }
    printf("syntex error(%d): expect \"%s\" but input \"%s\"\n",line_loc, expect_l[tok_check & TOKEN_MASK], yytext); 
    return 0;
}

AST* input(AST* in)
{
    AST* ret;

    if(tok == VAR
    || tok == LPAREN
    || tok == SUB_FUNC
    || tok == INT
    || tok == REAL
    || tok == STR
    || tok == ADD
    || tok == SUB
    ) {
        DORET(assign);
    } else {
        return 0;
    }
    return ret;
}

AST* assign(AST* in)
{
    AST* ret;

    if(tok == VAR){
        AST* var = ast_new(VAR, yytext, yyleng);
        next_tok();
        in = var;
        DORET(assignd);
    } else if(tok == ADD || tok == SUB) {
        AST* unary;
        DO(unaryop, unary);
        DORET(exp_unary);
        ast_add_node(unary, 0, ret);

        DOEPS(exp_muld, unary);
        DOEPS(exp_addd, ret);
    } else if(tok == LPAREN
            || tok == SUB_FUNC
            || tok == INT
            || tok == REAL
            || tok == STR) {
        DORET(factor_etc);
        DOEPS(exp_muld, ret);
        DOEPS(exp_addd, ret);
        
    } else {
        return 0;
    }
    return ret;
}

AST* assignd(AST* in)
{
    AST* ret;

    if(tok == ASSIGN) {
        AST* assign_ast = ast_new(ASSIGN, (void*)'=', 0);
        next_tok();
        DORET(assign);
        ast_add_node(assign_ast, 1, ret);
        ret = assign_ast;
    } else if(tok == DOLLOR
        || tok == RPAREN
        || tok == ADD
        || tok == SUB
        || tok == MUL
        || tok == DIV) {
        DOEPS(exp_muld, in);
        DOEPS(exp_addd, ret);
    } else {
        return 0;
    }
    return ret;
}

AST* exp_add(AST* in)
{
    AST* ret;

    if(tok == VAR
    || tok == LPAREN
    || tok == SUB_FUNC
    || tok == INT
    || tok == REAL
    || tok == STR
    || tok == ADD
    || tok == SUB) {
        DO(exp_mul, in);
        DOEPS(exp_addd, in);
    } else {
        return 0;
    }
    return ret;
}

AST* exp_addd(AST* in)
{
    AST* ret;

    if(tok == DOLLOR
    || tok == RPAREN
    || tok == COMMA) {
        return SKIP;
    } else if(tok == ADD
            || tok == SUB) {
        AST* adds;
        DO(addop, adds);
        ast_add_node(adds, 0, in);

        DORET(exp_mul);
        ast_add_node(adds, 1, ret);
        
        DOEPS(exp_addd, adds);
    } else {
        return 0;
    }
    return ret;
}

AST* exp_mul(AST* in)
{
    AST* ret;

    if(tok == VAR
    || tok == LPAREN
    || tok == SUB_FUNC
    || tok == INT
    || tok == REAL
    || tok == STR
    || tok == ADD
    || tok == SUB) {
        DO(exp_unary, in);
        DOEPS(exp_muld, in);
    } else {
        return 0;
    }
    return ret;
}

AST* exp_muld(AST* in)
{
    AST* ret;

    if(tok == DOLLOR
    || tok == RPAREN
    || tok == COMMA
    || tok == ADD
    || tok == SUB) {
        return SKIP;
    } else if(tok == MUL
            || tok == DIV) {
        AST* muls;
        DO(mulop, muls);
        ast_add_node(muls, 0, in);

        DORET(exp_unary);
        ast_add_node(muls, 1, ret);
        
        DOEPS(exp_muld, muls);
    } else {
        return 0;
    }
    return ret;
}

AST* exp_unary(AST* in)
{
    AST* ret;

    if(tok == VAR
    || tok == LPAREN
    || tok == SUB_FUNC
    || tok == INT
    || tok == REAL
    || tok == STR) {
        DORET(factor_var);
    } else if(tok == ADD
            || tok == SUB) {
        AST* unary;
        DO(unaryop, unary);
        DORET(exp_unary);
        ast_add_node(unary, 0, ret);
        ret = unary;
    } else {
        return 0;
    }
    return ret;
}

AST* factor_var(AST* in)
{
    AST* ret;

    if(tok == VAR) {
        ret = ast_new(VAR, yytext, yyleng);
        next_tok();
    } else if(tok == LPAREN
            || tok == SUB_FUNC
            || tok == INT
            || tok == REAL
            || tok == STR) {
        DORET(factor_etc);
    } else {
        return 0;
    }
    return ret;
}

AST* factor_etc(AST* in)
{
    AST* ret = 0;

    if(tok == LPAREN) {
        next_tok();
        DORET(assign);
        expect(RPAREN);
    } else if(tok == SUB_FUNC) {
        next_tok();
        AST* sub_func = ast_new(SUB_FUNC, 0, 0);
        DORET(exp_add);
        ast_add_node(sub_func, 0, ret);
        expect(COMMA);
        DORET(exp_add);
        ast_add_node(sub_func, 1, ret);
        expect(COMMA);
        DORET(exp_add);
        ast_add_node(sub_func, 2, ret);
        expect(RPAREN);
        ret = sub_func;
    } else if(tok == INT) {
        int i;
        sscanf(yytext, "%d", &i);
        ret = ast_new(INT, &i, 0);
        next_tok();
    } else if(tok == REAL) {
        double f;
        sscanf(yytext, "%lf", &f);
        ret = ast_new(REAL, &f, 0);
        next_tok();
    } else if(tok == STR) {
        ret = ast_new(STR, yytext, yyleng);
        next_tok();
    } else {
        return 0;
    }
    return ret;
}

AST* mulop(AST* in)
{
    AST* ret;

    if(tok == MUL) {
        ret = ast_new(MUL, (void*)'*', 0);
        next_tok();
    } else if(tok == DIV) {
        ret = ast_new(DIV, (void*)'/', 0);
        next_tok();
    } else {
        return 0;
    }
    return ret;
}

AST* addop(AST* in)
{
    AST* ret;

    if(tok == ADD) {
        ret = ast_new(ADD, (void*)'+', 0);
        next_tok();
    } else if(tok == SUB) {
        ret = ast_new(SUB, (void*)'-', 0);
        next_tok();
    } else {
        return 0;
    }
    return ret;
}

AST* unaryop(AST* in)
{
    AST* ret;

    if(tok == ADD) {
        int tmp = 1;
        ret = ast_new(UNARY, &tmp, 0);
        next_tok();
    } else if(tok == SUB) {
        int tmp = -1;
        ret = ast_new(UNARY, &tmp, 0);
        next_tok();
    } else {
        return 0;
    }
    return ret;
}

AST* parser(AST* in)
{
    next_tok();
    return input(in);
}

void process_input(char* input_buffer)
{
    void* lex_string_buffer = yy_scan_string(input_buffer);

    AST* ast_root;
    
    line_loc = 0;
    yyleng = 0;
    tok = DOLLOR;

    yy_switch_to_buffer(lex_string_buffer);
    ast_root = parser(NULL);
    
    yy_delete_buffer(lex_string_buffer);
    if(ast_root)
        ast_print(ast_root);

    ast_free_all(ast_root);
}
