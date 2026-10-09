#include <stdio.h>
#include "lexer.c"

int main(int argc, char ** argv)
{
    char *line = NULL;
    size_t size;
    ssize_t len = getline(&line, &size, stdin);

    tokenize(line, len);

    for (int i = 0; i < tokens_len; i++)
    {
        printf("%.*s\n", (int)tokens[i]->token_len, tokens[i]->token_pos);
    }
}