#include <stdio.h>
#include <stdint.h>
#include <ctype.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>


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
    TOK_NUMBER,
    TOK_PLUS,
    TOK_MINUS,
    TOK_DIV,
    TOK_MULT,
    Eof
} TOKEN_TYPE;



typedef struct Token {
    TOKEN_TYPE type;
    char *token_pos;
    size_t token_len;
    int number_value;
} Token;

Token *new_token(TOKEN_TYPE type, char *pos, size_t len, int number_value)
{
    Token *tok = malloc(sizeof(Token));
    tok->type = type;
    tok->token_pos = pos;
    tok->token_len = len;
    tok->number_value = number_value;
    return tok;
}

Token *tokens[256] = {0};
size_t tokens_len = 0;
void tokens_append(Token *tok)
{
    assert(tokens_len + 1 != sizeof(tokens) / sizeof(Token *));
    tokens[tokens_len++] = tok;
}

size_t lexer_i = 0;
Token *lexer_next() 
{ 
    if (lexer_i < 256)
        return tokens[lexer_i++]; 
}

Token *lexer_peek() { 
    if (lexer_i < 256)
        return tokens[lexer_i]; 
}

typedef struct Tokenize {
    bool in_token;
    char *token_begin;
    size_t token_len;
    int number_value;
    TOKEN_TYPE token_state;
} Tokenize;

void tokenize_begin(Tokenize *tokenize, char *pos, TOKEN_TYPE type)
{
    tokenize->in_token = true;
    tokenize->token_begin = pos;
    tokenize->token_len = 0;
    tokenize->token_state = type;
    tokenize->number_value = 0;
}

void tokenize_next(Tokenize *tokenize)
{
    tokenize->token_len++;
}

Token *tokenize_end(Tokenize *tokenize)
{
    if (tokenize->token_state == TOK_NUMBER)
    {
        char num[tokenize->token_len + 1];
        memcpy(num, (void *) tokenize->token_begin, tokenize->token_len);
        num[tokenize->token_len + 1] = '\0';
        tokenize->number_value = strtoul(num, NULL, 10);
    }

    Token * token = new_token(tokenize->token_state, tokenize->token_begin, tokenize->token_len, tokenize->number_value);
    tokenize_begin(tokenize, NULL, Eof);
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
    tokenize_begin(&tokenize, NULL , Eof);

    for (int i = 0; i < len; i++)
    {
        char *c = &input[i];
        switch (tokenize.token_state)
        {
            case Eof: // beginning
                switch (*c)
                {
                    case DIGIT:
                        tokenize_begin(&tokenize, c, TOK_NUMBER);
                        tokenize_next(&tokenize);
                        break;    
        
                    case '+':
                        tokens_append(new_token(TOK_PLUS, c, 1, 0));
                        break;

                    case '-':
                        tokens_append(new_token(TOK_MINUS, c, 1, 0));
                        break;

                    case '*':
                        tokens_append(new_token(TOK_MULT, c, 1, 0));
                        break;

                    case '/':
                        tokens_append(new_token(TOK_DIV, c, 1, 0));
                        break;

                    case WHITESPACE:
                        break;
                
                }

                break;
            
            case TOK_NUMBER:
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

    tokens_append(tokenize_end(&tokenize));

    return 0;

}

