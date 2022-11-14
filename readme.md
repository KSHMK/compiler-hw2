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

exp_unary   : unaryop exp_unary
            | factor

factor      : LPAREN assign RPAREN
            | SUB_FUNC exp COMMA exp COMMA exp RPAREN
            | INT 
            | REAL 
            | STR
            | VAR


addop       : ADD | SUB
mulop       : MUL | DIV
unaryop     : ADD | SUB
```

## CONVERT LEFT ASSO

```
input       : assign

assign      : VAR assign'
            | unary exp_unary exp_mul' exp_add' 
            | factor_etc exp_mul' exp_add' 

assign'     : = assign 
            | exp_mul' exp_add'

exp_add     : exp_mul exp_add'
exp_add'    : addop exp_mul exp_add' 
            | ε


exp_mul     : exp_unary exp_mul' 
exp_mul'    : mulop exp_unary exp_mul' 
            | ε

exp_unary   : unaryop exp_unary 
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

unaryop     : ADD 
            | SUB
```

## OPT
```
input       : assign

assign      : VAR assign'
            | unaryop exp_unary exp_mul' exp_add' 
            | factor_etc exp_mul' exp_add' 

assign'     : = assign 
            | exp_mul' exp_add'

exp_add     : exp_mul exp_add' 
exp_add'    : addop exp_mul exp_add' 
            | ε


exp_mul     : exp_unary exp_mul' 
exp_mul'    : mulop exp_unary exp_mul' 
            | ε

exp_unary   : addop exp_unary 
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

unaryop     : ADD 
            | SUB
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