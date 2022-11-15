#define TERM        0x00000000
#define OPERATOR    0x00010000
#define VALUE       0x00100000
#define TOKEN_MASK  0x0000ffff

#define DOLLOR      ( 0     | TERM )
#define VAR         ( 1     | TERM | VALUE )
#define STR         ( 2     | TERM | VALUE )
#define INT         ( 3     | TERM | VALUE )
#define REAL        ( 4     | TERM | VALUE )
#define ADD         ( 5     | TERM | OPERATOR )
#define SUB         ( 6     | TERM | OPERATOR )
#define MUL         ( 7     | TERM | OPERATOR )
#define DIV         ( 8     | TERM | OPERATOR )
#define LPAREN      ( 9     | TERM )
#define RPAREN      ( 10    | TERM )
#define SUB_FUNC    ( 11    | TERM | OPERATOR )
#define COMMA       ( 12    | TERM )
#define ASSIGN      ( 13    | TERM | OPERATOR )
#define UNARY       ( 14    | TERM | OPERATOR )
#define ERR         ( 255   | TERM )
