#include <stdio.h>
#include <stdint.h>
#include <ctype.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>


#define WHITESPACE  \
    ' ':            \
    case '\n'

#define DIGIT \
    '0': \
    case '1': \
    case '2': \
    case '3': \
    case '4': \
    case '5': \
    case '6': \
    case '7': \
    case '8': \
    case '9'

typedef enum TOKEN_TYPE {
    NUMBER,
    PLUS,
    MINUS,
    DIV,
    MULT,
    END_OF_FILE
} TOKEN_TYPE;



typedef struct Token {
    TOKEN_TYPE type;
    char *token_pos;
    size_t token_len;
} Token;

Token *new_token(TOKEN_TYPE type, char *pos, size_t len)
{
    Token *tok = malloc(sizeof(Token));
    tok->type = type;
    tok->token_pos = pos;
    tok->token_len = len;
    return tok;
}

Token *tokens[256];
size_t tokens_len = 0;
void tokens_append(Token *tok)
{
    assert(tokens_len + 1 != sizeof(tokens) / sizeof(Token *));
    tokens[tokens_len++] = tok;
}

typedef struct Tokenize {
    bool in_token;
    char *token_begin;
    size_t token_len;
    TOKEN_TYPE token_state;
} Tokenize;

void tokenize_begin(Tokenize *tokenize, char *pos, TOKEN_TYPE type)
{
    tokenize->in_token = true;
    tokenize->token_begin = pos;
    tokenize->token_len = 0;
    tokenize->token_state = type;
}

void tokenize_next(Tokenize *tokenize)
{
    tokenize->token_len++;
}

Token *tokenize_end(Tokenize *tokenize)
{
    Token * token = new_token(tokenize->token_state, tokenize->token_begin, tokenize->token_len);
    tokenize_begin(tokenize, NULL, END_OF_FILE);
    return token;
}

void tokenize_error(Tokenize *tokenize)
{
    assert(&tokenize->token_begin[tokenize->token_len]);
    printf("Lexing Error: Unexpected Character: %c\n", tokenize->token_begin[tokenize->token_len]);
}

int tokenize(char *input, size_t len)
{
    Tokenize tokenize;
    tokenize_begin(&tokenize, NULL , END_OF_FILE);

    for (int i = 0; i < len; i++)
    {
        char *c = &input[i];
        switch (tokenize.token_state)
        {
            case END_OF_FILE: // beginning
                switch (*c)
                {
                    case DIGIT:
                        tokenize_begin(&tokenize, c, NUMBER);
                        tokenize_next(&tokenize);
                        break;    
        
                    case '+':
                        tokens_append(new_token(PLUS, c, 1));
                        break;

                    case '-':
                        tokens_append(new_token(MINUS, c, 1));
                        break;

                    case '*':
                        tokens_append(new_token(MULT, c, 1));
                        break;

                    case '/':
                        tokens_append(new_token(DIV, c, 1));
                        break;

                    case WHITESPACE:
                        break;
                
                }

                break;
            
            case NUMBER:
                switch (*c)
                {
                    case DIGIT:
                        tokenize_next(&tokenize);
                        break;
                    
                    default:
                        tokens_append(tokenize_end(&tokenize));
                        i--;
                        break;
                }

                break;

        default:
            break;
        }
    }

    return 0;

}

