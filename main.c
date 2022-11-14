#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "token.h"
#include "parser.h"
#include "process.h"

AST* ast_root = NULL;

extern int yylex_destroy ( void );
int run_input(char* input_buffer);

int main(int argc, char* argv[])
{
    char input_buffer[200];
    int input_size;

    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
    printf("compiler hw2 interpreter\n");

    var_list = vl_init();
    
    while(1) {
        printf("> ");
        
        if((input_size = read(0, input_buffer, 200)) <= 0)
            break;
        input_buffer[input_size-1] = 0;

        if(run_input(input_buffer))
            break;
    }

    yylex_destroy();
    vl_free(var_list);
    return 0;
}

int run_input(char* input_buffer)
{
    TOKEN* token_root;
    AST* ast;
    AST* result;

    if(!strncmp(input_buffer, "$exit", 5))
    {
        ast_free_all(ast_root);
        return 1;
    }
    if(!strncmp(input_buffer, "$ast", 4))
    {
        ast_print(ast_root);
        return 0;
    }
    if(!strncmp(input_buffer, "$symbol", 7))
    {
        vl_print(var_list);
        return 0;
    } 

    if(!(token_root = lexing_input(input_buffer)))
        return 0;

    if(!(ast = parse_input(token_root)))
    {
        tl_free(token_root);
        return 0;
    }
    tl_free(token_root);
    
    if(!(result = process_ast(ast)))
    {
        ast_free_all(ast);
        return 0;
    }

    ast_print(result);
    ast_free_all(result);

    if(ast_root)
        ast_free_all(ast_root);
    ast_root = ast;

    return 0;
}
