#define TERM        0x00000000
#define NEED_AST    0x00010000
#define OPERATOR    0x00020000
#define TOKEN_MASK  0x0000ffff
#define DOLLOR      ( 0     | TERM )
#define VAR         ( 1     | TERM | NEED_AST )
#define STR         ( 2     | TERM | NEED_AST )
#define INT         ( 3     | TERM | NEED_AST )
#define REAL        ( 4     | TERM | NEED_AST )
#define ADD         ( 5     | TERM | NEED_AST | OPERATOR )
#define SUB         ( 6     | TERM | NEED_AST | OPERATOR )
#define MUL         ( 7     | TERM | NEED_AST | OPERATOR )
#define DIV         ( 8     | TERM | NEED_AST | OPERATOR )
#define LPAREN      ( 9     | TERM )
#define RPAREN      ( 10    | TERM )
#define SUB_FUNC    ( 11    | TERM | NEED_AST | OPERATOR )
#define COMMA       ( 12    | TERM )
#define ASSIGN      ( 13    | TERM | NEED_AST | OPERATOR )
#define ERR         ( 255   | TERM )

#define NON_TERM        0x10000000
#define NT_INPUT        ( 1   | NON_TERM )
#define NT_ASSIGN       ( 2   | NON_TERM )
#define NT_ASSIGNd      ( 3   | NON_TERM )
#define NT_EXP_ADD      ( 4   | NON_TERM )
#define NT_EXP_ADDd     ( 5   | NON_TERM )
#define NT_EXP_MUL      ( 6   | NON_TERM )
#define NT_EXP_MULd     ( 7   | NON_TERM )
#define NT_EXP_UNARY    ( 8   | NON_TERM )
#define NT_FACTOR_VAR   ( 9   | NON_TERM )
#define NT_FACTOR_ETC   ( 10  | NON_TERM )
#define NT_ADDOP        ( 11  | NON_TERM )
#define NT_MULOP        ( 12  | NON_TERM )

