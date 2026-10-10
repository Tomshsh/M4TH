#include "lexer.c"

#define parser_assert(condition, message)   \
    if (!condition) \
    {   \
        printf("%s %s %d\n", message, __FILE__, __LINE__);  \
        exit(1);    \
    }  

typedef enum ExprType {
    EXPR_BINARY, EXPR_NUMBER
} ExprType;

typedef struct Expr Expr;

typedef struct BinaryExpr {
    Expr *right;
    Expr *left;
    Token op;
} BinaryExpr;

typedef struct NumberExpr {
    Token num;
} NumberExpr;

struct Expr {
    union{
        BinaryExpr binary;
        NumberExpr number;
    };

    ExprType type;
};

Expr *new_expression()
{
    Expr *expression = malloc(sizeof(Expr));
    return expression;
}

Expr *new_binary(Expr *left, Expr *right, Token op)
{
    Expr *expr = new_expression();
    expr->binary.left = left;
    expr->binary.right = right;
    expr->binary.op = op;
    expr->type = EXPR_BINARY;
    return expr;
}

Expr *new_number(Token tok)
{
    Expr *expr = new_expression();
    expr->number.num = tok;
    expr->type = EXPR_NUMBER;
    return expr;
}

Expr *parse_expression()
{
    Token left = *lexer_next();
    parser_assert(left.type == TOK_NUMBER, "bad token!");
    
    Expr *lhs = new_number(left);
    
    Token operator = *lexer_peek();
    parser_assert(operator.type != TOK_NUMBER, "bad_token!");

    if (operator.type != Eof)
    {
        operator = *lexer_next();
        Expr *rhs = parse_expression();
        return new_binary(lhs, rhs, operator);
    }
    
    return lhs;
}

int resolve_expression(Expr *expr)
{
    if (expr->type == EXPR_NUMBER)
        return expr->number.num.number_value;

    int left = resolve_expression(expr->binary.left);
    int right = resolve_expression(expr->binary.right);

    switch (expr->binary.op.type)
    {
        case TOK_PLUS:
            return left + right;  
        case TOK_MINUS:
            return left - right;
        case TOK_DIV:
            return left / right;
        case TOK_MULT:
            return left * right;
        
        default:
            break;
    }
}