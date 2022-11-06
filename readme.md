# Grammer
```
input       : assign
assign      : assign = exp | exp

exp         : exp_add

exp_add     : exp_add addop exp_mul
            | exp_mul

exp_mul     : exp_mul mulop unary
            | unary

exp_unary   : addop factor
            | factor

factor      : LPAREN assign RPAREN
            | SUB_FUNC exp COMMA exp COMMA exp RPAREN
            | INT 
            | REAL 
            | STR
            | VAR


addop       : ADD | SUB
mulop       : MUL | DIV
```

# LEX
```
VAR         [a-zA-Z][0-9a-zA-Z]
STR         
INT         
REAL        
ADD         \+ 
SUB         \-
MUL         \*
DIV         \/
LPAREN      \(
RPAREN      \)
SUB_FUNC    sub\(
COMMA       \,
```