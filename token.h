#define TERM        0x00000000
#define TOKEN_MASK  0x0000ffff
#define DOLLOR      ( 0     | TERM )
#define VAR         ( 1     | TERM )
#define STR         ( 2     | TERM )
#define INT         ( 3     | TERM )
#define REAL        ( 4     | TERM )
#define ADD         ( 5     | TERM )
#define SUB         ( 6     | TERM )
#define MUL         ( 7     | TERM )
#define DIV         ( 8     | TERM )
#define LPAREN      ( 9     | TERM )
#define RPAREN      ( 10    | TERM )
#define SUB_FUNC    ( 11    | TERM )
#define COMMA       ( 12    | TERM )
#define ASSIGN      ( 13    | TERM )
#define UNARY       ( 14    | TERM )
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

