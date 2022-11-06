#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "token.h"

unsigned charCount = 0, wordCount = 0, lineCount = 0;

extern char *yytext;
extern int yyleng;
extern int yylex();

extern void* yy_scan_string( const char *yy_str  );
extern void yy_switch_to_buffer ( void* new_buffer  );
extern void yy_delete_buffer ( void* b  );


void parser(char* input_buffer, int input_size);

int main(int argc, char* argv[])
{
    char input_buffer[200];
    int input_size;
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
    printf("compiler hw2 interpreter\n");
    while(1) {
        printf("> ");
        //getline();
        if((input_size = read(0, input_buffer, 200)) <= 0)
            break;
        input_buffer[input_size-1] = 0;
        
        void* lex_string_buffer = yy_scan_string(input_buffer);


        printf("%s\n%d\n",input_buffer,input_size);
        yy_switch_to_buffer(lex_string_buffer);
        parser(input_buffer, input_size);
        yy_delete_buffer(lex_string_buffer);
        
    }
    return 0;
}

void parser(char* input_buffer, int input_size)
{
    int tok;
    char *token[] = {"","VAR",
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
"ASSIGN",};
    while((tok=yylex())!=0){
        if(tok == ERR)
            printf("ERR\n");
        else
            printf("%s %d %s\n",yytext, yyleng, token[tok]);
    }
        
}