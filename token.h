#define OPERATOR    0x00010000
#define VALUE       0x00100000
#define TOKEN_MASK  0x0000ffff

#define DOLLOR      ( 0     )
#define VAR         ( 1     | VALUE )
#define STR         ( 2     | VALUE )
#define INT         ( 3     | VALUE )
#define REAL        ( 4     | VALUE )
#define ADD         ( 5     | OPERATOR )
#define SUB         ( 6     | OPERATOR )
#define MUL         ( 7     | OPERATOR )
#define DIV         ( 8     | OPERATOR )
#define LPAREN      ( 9     )
#define RPAREN      ( 10    )
#define SUB_FUNC    ( 11    | OPERATOR )
#define COMMA       ( 12    )
#define ASSIGN      ( 13    | OPERATOR )
#define UNARY       ( 14    | OPERATOR )
#define SPACE       ( 15    )
#define ERR         ( 255   )
