#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "token.h"
#include "parser.h"

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
        
        if((input_size = read(0, input_buffer, 200)) <= 0)
            break;
        input_buffer[input_size-1] = 0;
        
        if(!strncmp(input_buffer, "$exit", 5))
            break;

        process_input(input_buffer);        
    }

    return 0;
}


