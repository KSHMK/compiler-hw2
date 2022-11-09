#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

void error_handler(char* file, int line, char* msg)
{
    printf("FILE:%s:%d ERROR: %s\n", file, line, msg);
    exit(-1);
}
