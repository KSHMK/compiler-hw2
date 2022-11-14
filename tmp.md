input ::= assign
assign ::= VAR assign'
assign ::= addop factor_var exp_mul' exp_add'
assign ::= factor_etc exp_mul' exp_add'
assign' ::= = assign
assign' ::= exp_mul' exp_add'
exp_add ::= exp_mul exp_add'
exp_add' ::= addop exp_mul exp_add'
exp_add' ::= ''
exp_mul ::= exp_unary exp_mul'
exp_mul' ::= mulop exp_unary exp_mul'
exp_mul' ::= ''
exp_unary ::= addop exp_unary
exp_unary ::= factor_var
factor_var ::= VAR
factor_var ::= factor_etc
factor_etc ::= LPAREN assign RPAREN
factor_etc ::= SUB_FUNC exp_add COMMA exp_add COMMA exp_add RPAREN
factor_etc ::= INT
factor_etc ::= REAL
factor_etc ::= STR
addop ::= ADD
addop ::= SUB
mulop ::= MUL
mulop ::= DIV


