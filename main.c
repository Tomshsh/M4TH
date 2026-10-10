#include <stdio.h>
#include "parser.c"

int main(int argc, char ** argv)
{
    char *line = NULL;
    size_t size;
    ssize_t len = getline(&line, &size, stdin);

    tokenize(line, len);

    int result = resolve_expression(parse_expression(0));

    printf("%d\n", result);

}