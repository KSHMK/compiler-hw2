%{

#include "token.h"

%}

var     [a-zA-Z][0-9a-zA-Z]*
str     \"([^"]|\\\")*\"
int     [0-9]+
real    [0-9]*\.[0-9]+

%%

{var}       {return VAR;}
{str}       {return STR;}
{int}       {return INT;}
{real}      {return REAL;}
\+          {return ADD;}
\-          {return SUB;}
\*          {return MUL;}
\/          {return DIV;}
\(          {return LPAREN;}
\)          {return RPAREN;}
sub\(       {return SUB_FUNC;}
\,          {return COMMA;}
\=          {return ASSIGN;}
[ ]         {return SPACE;}
.           {return ERR;}
%%