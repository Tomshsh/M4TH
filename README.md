## This is just a calculator (but implemented like an interpreter)
I decided to make this project as a personal middle phase after experiencing
difficulties trying to implement the parser for a real language interpreter.

#### This is a very basic implementation of an interpreter.
```bash
# use any combination of numbers and operators in any sequence, spaces are ignored
$ ./out

3 / 4/ 5-2-2 *2
```
* The parser employs the *Pratt Algorithm* to recursivly construct an Expression tree (or AST).
* the resolver recursively travels the tree and calculates the result.

#### Current Limits
* The lexer defines only integer decimal digits and some term and factor operations as valid tokens.
