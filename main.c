#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "token.h"
#include "data_set.h"

extern char *yytext;
extern int yyleng;
extern int yylex();

extern void* yy_scan_string( const char *yy_str  );
extern void yy_switch_to_buffer ( void* new_buffer  );
extern void yy_delete_buffer ( void* b  );

PARSING_TABLE* pt = NULL;

void process_input(char* input_buffer);
int parser(void);

int main(int argc, char* argv[])
{
    char input_buffer[200];
    int input_size;

    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
    printf("compiler hw2 interpreter\n");

    pt = pt_build();

    while(1) {
        printf("> ");
        
        if((input_size = read(0, input_buffer, 200)) <= 0)
            break;
        input_buffer[input_size-1] = 0;
        
        if(!strncmp(input_buffer, "$exit", 5))
            break;

        process_input(input_buffer);        
    }

    if(pt)
        pt_release(pt);
    return 0;
}

void process_input(char* input_buffer)
{
    void* lex_string_buffer = yy_scan_string(input_buffer);
    int result;
    yy_switch_to_buffer(lex_string_buffer);
    result = parser();
    yy_delete_buffer(lex_string_buffer);

    if(!result)
        return;
}

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

int parser(void)
{
    int tok;
    int line_loc = 1;
    int state = 1;
    int node = 0;
    
    STACK* stack;
    stack = stack_init();
    stack_push(stack,DOLLOR);
    stack_push(stack,NT_INPUT);
    tok = yylex();
    while(1) {
        state = stack_pop(stack);
        //printf("%08x %08x\n",state, tok);
        if((state & NON_TERM) != NON_TERM)
        {
            if(tok != state) {
                printf("syntex error(%d): %s\n",line_loc, yytext);
                goto _FAIL;
            }
            if(tok == 0)
                break;
            printf("%s\n",tok_l[tok]);
            tok = yylex();
            if(tok == ERR)
            {
                printf("lexical error(%d): %s\n",line_loc, yytext);
                goto _FAIL;
            }
            line_loc += yyleng;
            continue;
        }

        node = pt->table[state & ~NON_TERM][tok];
        if(node == 0)
        {
            printf("syntex error(%d): %s\n",line_loc, yytext);
            goto _FAIL;
        }
        
        else{
            for(int i=pt->node[node]->len - 1; i>=0; i--)
                stack_push(stack, pt->node[node]->list[i]);
        }
    }
        

_FAIL:
    stack_free(stack);
        
}