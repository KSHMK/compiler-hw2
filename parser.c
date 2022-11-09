#include <stdio.h>
#include "parser.h"
#include "token.h"

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

#define DO(x)   if(!(ret=x())){return 0;}
int tok;
int line_loc;

int next_tok(void)
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

int accept(int tok_check)
{
    if(tok == tok_check)
        return next_tok();
    return 0;
}

int expect(int tok_check)
{
    if(accept(tok_check))
        return 1;
    return 0;
}

int input(void)
{
    int ret;

    if(tok == VAR
    || tok == LPAREN
    || tok == SUB_FUNC
    || tok == INT
    || tok == REAL
    || tok == STR
    || tok == ADD
    || tok == SUB
    ) {
        DO(assign);
    } else {
        return 0;
    }
    return 1;
}

int assign(void)
{
    int ret;

    if(accept(VAR)){
        DO(assignd);
    } else if(tok == ADD || tok == SUB) {
        DO(addop);
        DO(factor_var);
        DO(exp_muld);
        DO(exp_addd);
    } else if(tok == LPAREN
            || tok == SUB_FUNC
            || tok == INT
            || tok == REAL
            || tok == STR) {
        DO(factor_etc);
        DO(exp_muld);
        DO(exp_addd);
    } else {
        return 0;
    }
    return 1;
}

int assignd(void)
{
    int ret;

    if(accept(ASSIGN)) {
        DO(assign);
    } else if(tok == DOLLOR
        || tok == RPAREN
        || tok == ADD
        || tok == SUB
        || tok == MUL
        || tok == DIV) {
        DO(exp_muld);
        DO(exp_addd);
    } else {
        return 0;
    }
    return 1;
}

int exp_add(void)
{
    int ret;

    if(tok == VAR
    || tok == LPAREN
    || tok == SUB_FUNC
    || tok == INT
    || tok == REAL
    || tok == STR
    || tok == ADD
    || tok == SUB) {
        DO(exp_mul);
        DO(exp_addd);
    } else {
        return 0;
    }
    return 1;
}

int exp_addd(void)
{
    int ret;

    if(tok == DOLLOR
    || tok == RPAREN
    || tok == COMMA) {
        ;
    } else if(tok == ADD 
            || tok == SUB) {
        DO(addop);
        DO(exp_mul);
        DO(exp_addd);
    } else {
        return 0;
    }
    return 1;
}

int exp_mul(void)
{
    int ret;

    if(tok == VAR
    || tok == LPAREN
    || tok == SUB_FUNC
    || tok == INT
    || tok == REAL
    || tok == STR
    || tok == ADD
    || tok == SUB) {
        DO(exp_unary);
        DO(exp_muld);
    } else {
        return 0;
    }
    return 1;
}

int exp_muld(void)
{
    int ret;

    if(tok == DOLLOR
    || tok == RPAREN
    || tok == COMMA
    || tok == ADD
    || tok == SUB) {
        ;
    } else if(tok == MUL 
            || tok == DIV) {
        DO(mulop);
        DO(exp_unary);
        DO(exp_muld);
    } else {
        return 0;
    }
    return 1;
}

int exp_unary(void)
{
    int ret;

    if(tok == VAR
    || tok == LPAREN
    || tok == SUB_FUNC
    || tok == INT
    || tok == REAL
    || tok == STR) {
        DO(factor_var);
    } else if(tok == ADD
            || tok == SUB) {
        DO(addop);
        DO(factor_var);
    } else {
        return 0;
    }
    return 1;
}

int factor_var(void)
{
    int ret;

    if(accept(VAR)) {
        ;
    } else if(tok == LPAREN
            || tok == SUB_FUNC
            || tok == INT
            || tok == REAL
            || tok == STR) {
        DO(factor_etc);
    } else {
        return 0;
    }
    return 1;
}

int factor_etc(void)
{
    int ret;

    if(accept(LPAREN)) {
        DO(assign);
        expect(RPAREN);
    } else if(accept(SUB_FUNC)) {
        DO(exp_add);
        expect(COMMA);
        DO(exp_add);
        expect(COMMA);
        DO(exp_add);
        expect(RPAREN);
    } else if(accept(INT)) {
        ;
    } else if(accept(REAL)) {
        ;
    } else if(accept(STR)) {
        ;
    } else {
        return 0;
    }
    return 1;
}

int addop(void)
{
    int ret;

    if(accept(ADD)) {
        ;
    } else if(accept(SUB)) {
        ;
    } else {
        return 0;
    }
    return 1;
}

int mulop(void)
{
    int ret;

    if(accept(MUL)) {
        ;
    } else if(accept(DIV)) {
        ;
    } else {
        return 0;
    }
    return 1;
}

int parser(void)
{
    next_tok();
    input();
}

void process_input(char* input_buffer)
{
    void* lex_string_buffer = yy_scan_string(input_buffer);
    int result;
    yy_switch_to_buffer(lex_string_buffer);
    result = parser();
    printf("%d\n",result);
    yy_delete_buffer(lex_string_buffer);

    if(!result)
        return;
}
