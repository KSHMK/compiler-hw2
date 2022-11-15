#include <stdio.h>
#include <string.h>
#include "parser.h"
#include "token.h"
#include "data_set.h"

extern char *yytext;
extern int yyleng;
extern int yylex();

extern void* yy_scan_string( const char *yy_str  );
extern void yy_switch_to_buffer ( void* new_buffer  );
extern void yy_delete_buffer ( void* b  );


#define SKIP                            ((AST*)1)
#define PARSE_IN_OUT(func, in, out)     if(!(out=(func)(in))){return 0;}
#define PARSE_OUT(func, out)            PARSE_IN_OUT(func, in, out)
#define PARSE_RET(func)                 PARSE_OUT(func, ret)
#define PARSE_EPS(func, inp)            {in = inp; PARSE_RET(func); if(ret == SKIP){ret = in;}}
#define NEXT_TOKEN                      {cur_token = cur_token->next;}
#define EXPECT(token)                   {if(!expect(token)){return 0;}}      
#define SYNTEX_ERR_PRNT(fmt, ...)       {printf("syntex error: "); \
                                        printf(fmt, ##__VA_ARGS__); \
                                        printf("\n");}
TOKEN* cur_token;

int expect(int tok_check)
{
    if(cur_token->type == tok_check){
        NEXT_TOKEN;
        return 1;
    }
    SYNTEX_ERR_PRNT("예상 값 %s 실제 입력 값 %s", 
                    type_l[tok_check & TOKEN_MASK],
                    cur_token->text);
    return 0;
}

AST* input(AST* in)
{
    AST* ret;
    int tok = cur_token->type;
    
    if(tok == DOLLOR)
        return 0;
    
    if(tok == VAR
    || tok == LPAREN
    || tok == SUB_FUNC
    || tok == INT
    || tok == REAL
    || tok == STR
    || tok == ADD
    || tok == SUB
    ) {
        PARSE_RET(assign)
        EXPECT(DOLLOR);
    } 
    else {
        SYNTEX_ERR_PRNT("잘못된 입력 %s", type_l[tok & TOKEN_MASK]);
        return 0;
    }
    return ret;
}

AST* assign(AST* in)
{
    AST* ret;
    int tok = cur_token->type;

    if(tok == VAR){
        AST* var = ast_new(VAR, cur_token->text, cur_token->len);
        NEXT_TOKEN;
        PARSE_EPS(assignd, var);
    } else if(tok == ADD || tok == SUB) {
        AST* unary;
        PARSE_OUT(unaryop, unary);
        PARSE_EPS(exp_unary, unary);
        ast_add_node(unary, 0, ret);

        PARSE_EPS(exp_muld, unary);
        PARSE_EPS(exp_addd, ret);
    } else if(tok == LPAREN
            || tok == SUB_FUNC
            || tok == INT
            || tok == REAL
            || tok == STR) {
        PARSE_RET(factor_etc);
        PARSE_EPS(exp_muld, ret);
        PARSE_EPS(exp_addd, ret);
        
    } else {
        SYNTEX_ERR_PRNT("%s의 우항이 잘못됨", type_l[tok & TOKEN_MASK]);
        return 0;
    }
    return ret;
}

AST* assignd(AST* in)
{
    AST* ret;
    int tok = cur_token->type;

    if(tok == ASSIGN) {
        AST* assign_ast = ast_new(ASSIGN, (void*)'=', 0);
        ast_add_node(assign_ast, 0, in);
        NEXT_TOKEN;
        PARSE_RET(assign);
        ast_add_node(assign_ast, 1, ret);
        ret = assign_ast;
    } else if(tok == DOLLOR
        || tok == RPAREN
        || tok == ADD
        || tok == SUB
        || tok == MUL
        || tok == DIV) {
        PARSE_EPS(exp_muld, in);
        PARSE_EPS(exp_addd, ret);
    } else {
        SYNTEX_ERR_PRNT("%s 뒤에 잘못된 %s 출현", 
                        type_l[in->type & TOKEN_MASK],
                        type_l[tok & TOKEN_MASK]);
        return 0;
    }
    return ret;
}

AST* exp_add(AST* in)
{
    AST* ret;
    int tok = cur_token->type;

    if(tok == VAR
    || tok == LPAREN
    || tok == SUB_FUNC
    || tok == INT
    || tok == REAL
    || tok == STR
    || tok == ADD
    || tok == SUB) {
        PARSE_OUT(exp_mul, in);
        PARSE_EPS(exp_addd, in);
    } else {
        SYNTEX_ERR_PRNT("3");
        return 0;
    }
    return ret;
}

AST* exp_addd(AST* in)
{
    AST* ret;
    int tok = cur_token->type;

    if(tok == DOLLOR
    || tok == RPAREN
    || tok == COMMA) {
        return SKIP;
    } else if(tok == ADD
            || tok == SUB) {
        AST* adds;
        PARSE_OUT(addop, adds);
        ast_add_node(adds, 0, in);

        PARSE_RET(exp_mul);
        ast_add_node(adds, 1, ret);
        
        PARSE_EPS(exp_addd, adds);
    } else {
        SYNTEX_ERR_PRNT("4");
        return 0;
    }
    return ret;
}

AST* exp_mul(AST* in)
{
    AST* ret;
    int tok = cur_token->type;

    if(tok == VAR
    || tok == LPAREN
    || tok == SUB_FUNC
    || tok == INT
    || tok == REAL
    || tok == STR
    || tok == ADD
    || tok == SUB) {
        PARSE_OUT(exp_unary, in);
        PARSE_EPS(exp_muld, in);
    } else {
        SYNTEX_ERR_PRNT("+ 의 우항이 잘못됨");
        return 0;
    }
    return ret;
}

AST* exp_muld(AST* in)
{
    AST* ret;
    int tok = cur_token->type;

    if(tok == DOLLOR
    || tok == RPAREN
    || tok == COMMA
    || tok == ADD
    || tok == SUB) {
        return SKIP;
    } else if(tok == MUL
            || tok == DIV) {
        AST* muls;
        PARSE_OUT(mulop, muls);
        ast_add_node(muls, 0, in);

        PARSE_RET(exp_unary);
        ast_add_node(muls, 1, ret);
        
        PARSE_EPS(exp_muld, muls);
    } else {
        SYNTEX_ERR_PRNT("%s의 우항이 잘못됨", type_l[in->type & TOKEN_MASK]);
        return 0;
    }
    return ret;
}

AST* exp_unary(AST* in)
{
    AST* ret;
    int tok = cur_token->type;

    if(tok == VAR
    || tok == LPAREN
    || tok == SUB_FUNC
    || tok == INT
    || tok == REAL
    || tok == STR) {
        PARSE_RET(factor_var);
    } else if(tok == ADD
            || tok == SUB) {
        AST* unary;
        PARSE_OUT(unaryop, unary);
        PARSE_RET(exp_unary);
        ast_add_node(unary, 0, ret);
        ret = unary;
    } else {
        SYNTEX_ERR_PRNT("%s의 우항이 잘못됨", type_l[in->type & TOKEN_MASK]);
        return 0;
    }
    return ret;
}

AST* factor_var(AST* in)
{
    AST* ret;
    int tok = cur_token->type;

    if(tok == VAR) {
        ret = ast_new(VAR, cur_token->text, cur_token->len);
        NEXT_TOKEN;
    } else if(tok == LPAREN
            || tok == SUB_FUNC
            || tok == INT
            || tok == REAL
            || tok == STR) {
        PARSE_RET(factor_etc);
    } else {
        SYNTEX_ERR_PRNT("8");
        return 0;
    }
    return ret;
}

AST* factor_etc(AST* in)
{
    AST* ret = 0;
    int tok = cur_token->type;

    if(tok == LPAREN) {
        NEXT_TOKEN;
        PARSE_RET(assign);
        EXPECT(RPAREN);
    } else if(tok == SUB_FUNC) {
        NEXT_TOKEN;
        AST* sub_func = ast_new(SUB_FUNC, 0, 0);
        PARSE_RET(exp_add);
        ast_add_node(sub_func, 0, ret);
        EXPECT(COMMA);
        PARSE_RET(exp_add);
        ast_add_node(sub_func, 1, ret);
        EXPECT(COMMA);
        PARSE_RET(exp_add);
        ast_add_node(sub_func, 2, ret);
        EXPECT(RPAREN);
        ret = sub_func;
    } else if(tok == INT) {
        int i;
        sscanf(cur_token->text, "%d", &i);
        ret = ast_new(INT, &i, 0);
        NEXT_TOKEN;
    } else if(tok == REAL) {
        double r;
        sscanf(cur_token->text, "%lf", &r);
        ret = ast_new(REAL, &r, 0);
        NEXT_TOKEN;
    } else if(tok == STR) {
        ret = ast_new(STR, &cur_token->text[1], cur_token->len-2);
        NEXT_TOKEN;
    } else {
        SYNTEX_ERR_PRNT("8");
        return 0;
    }
    return ret;
}

AST* mulop(AST* in)
{
    AST* ret;
    int tok = cur_token->type;

    if(tok == MUL) {
        ret = ast_new(MUL, (void*)'*', 0);
        NEXT_TOKEN;
    } else if(tok == DIV) {
        ret = ast_new(DIV, (void*)'/', 0);
        NEXT_TOKEN;
    } else {
        SYNTEX_ERR_PRNT("9");
        return 0;
    }
    return ret;
}

AST* addop(AST* in)
{
    AST* ret;
    int tok = cur_token->type;

    if(tok == ADD) {
        ret = ast_new(ADD, (void*)'+', 0);
        NEXT_TOKEN;
    } else if(tok == SUB) {
        ret = ast_new(SUB, (void*)'-', 0);
        NEXT_TOKEN;
    } else {
        SYNTEX_ERR_PRNT("10");
        return 0;
    }
    return ret;
}

AST* unaryop(AST* in)
{
    AST* ret;
    int tok = cur_token->type;

    if(tok == ADD) {
        int tmp = 1;
        ret = ast_new(UNARY, &tmp, 0);
        NEXT_TOKEN;
    } else if(tok == SUB) {
        int tmp = -1;
        ret = ast_new(UNARY, &tmp, 0);
        NEXT_TOKEN;
    } else {
        SYNTEX_ERR_PRNT("11");
        return 0;
    }
    return ret;
}

AST* parser(AST* in)
{
    NEXT_TOKEN;
    return input(in);
}

TOKEN* lexing_input(char* input_buffer)
{
    int line_loc = 0;
    int tok, prev_tok;
    TOKEN *token_root, *token_prev, *token_new;
    void* lex_string_buffer = yy_scan_string(input_buffer);
    
    token_root = tl_new(DOLLOR, "", 0, NULL);
    token_prev = token_root;
    yyleng = 0;
    yy_switch_to_buffer(lex_string_buffer);
    
    while((tok = yylex())){
        if((token_prev->type == prev_tok) && ((token_prev->type & VALUE) && (tok & VALUE)))
        {
            printf("lexical error(%d): %s%s\n", line_loc-token_prev->len, token_prev->text, yytext);
            tl_free(token_root);
            yy_delete_buffer(lex_string_buffer);
            return NULL;
        }
        if(tok == ERR)
        {
            printf("lexical error(%d): %s", line_loc, yytext);
            while(yylex() == ERR)
                printf("%s", yytext);
            printf("\n");
            tl_free(token_root);
            yy_delete_buffer(lex_string_buffer);
            return NULL;
        }
        prev_tok = tok;
        if(tok == SPACE)
        {
            line_loc++;
            continue;
        }
        token_new = tl_new(tok, yytext, yyleng, token_prev);
        line_loc += yyleng;
        token_prev = token_new;
    }
    token_new = tl_new(DOLLOR, "", 0, token_prev);

    yy_delete_buffer(lex_string_buffer);
    return token_root;
}


AST* parse_input(TOKEN* token_root)
{
    AST* result;
    
    cur_token = token_root;
    result = parser(NULL);

    return result;
}