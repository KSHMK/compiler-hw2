# Grammer
## ORIGIN

```
input       : assign
assign      : VAR = assign | exp

exp         : exp_add

exp_add     : exp_add addop exp_mul
            | exp_mul

exp_mul     : exp_mul mulop exp_unary
            | exp_unary

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

## CONVERT LL(1)

```
input       : assign

assign      : VAR assign'
            | addop factor_var exp_mul' exp_add' 
            | factor_etc exp_mul' exp_add' 

assign'     : = assign 
            | exp_mul' exp_add'

exp_add     : exp_mul exp_add'
exp_add'    : addop exp_mul exp_add' 
            | ε


exp_mul     : exp_unary exp_mul' 
exp_mul'    : mulop exp_unary exp_mul' 
            | ε

exp_unary   : addop factor_var 
            | factor_var

factor_var  : VAR 
            | factor_etc

factor_etc  : LPAREN assign RPAREN
            | SUB_FUNC exp_add COMMA exp_add COMMA exp_add RPAREN
            | INT 
            | REAL 
            | STR

addop       : ADD 
            | SUB

mulop       : MUL 
            | DIV
```

# LEX
```
VAR         [a-zA-Z][0-9a-zA-Z]*
STR         \"([^"]|\\\")*\"
INT         [0-9]+
REAL        [0-9]*\.[0-9]+
ADD         \+ 
SUB         \-
MUL         \*
DIV         \/
LPAREN      \(
RPAREN      \)
SUB_FUNC    sub\(
COMMA       \,
```