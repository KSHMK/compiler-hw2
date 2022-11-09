#ifndef __PARSER_H__
#define __PARSER_H__

int input(void);
int assign(void);
int assignd(void);
int exp_add(void);
int exp_addd(void);
int exp_mul(void);
int exp_muld(void);
int exp_unary(void);
int factor_var(void);
int factor_etc(void);
int addop(void);
int mulop(void);

int parser(void);
void process_input(char* input_buffer);

#endif