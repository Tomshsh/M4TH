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
    tokens[++tokens_len] = tok;
}

int tokenize(char *input, size_t len)
{
    bool in_token = false;
    char *token_begin = NULL;
    TOKEN_TYPE token_state = END_OF_FILE;

    for (int i = 0; i < len; i++)
    {
        char c = input[i];
        switch (c)
        {
        case WHITESPACE:
            if (in_token)
                tokens_append(new_token(token_state, token_begin, &c - token_begin));

            in_token = false;
            break;
        
        case DIGIT:
            if (!in_token)
            {
                in_token = true;
                token_begin = &c;
                token_state = NUMBER;
            }
            break;    

        default:
            break;
        }
    }

    return 0;

}

