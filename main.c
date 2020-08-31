/*
 * This program converts an infix expression into the equivalent postfix expression
 * The input expression is passed as a string in argv[1] and NEEDS TO BE QUOTED
 * to remove the need for excaping on the shell (such as bash) the program is run from.
 *
 * Usage:
 * ./infix-postfix "<expr>"
*/

// include stack library
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// define a long enough token size
#define TOK_SZ 50

/* determines precedence of operator op:
 * {+, -} = 0
 * {*, /} = 1
 */
int precedence(char op)
{
    switch(op)
    {
    case '+':
    case '-':
        return 0;
    case '*':
    case '/':
        return 1;
    default:
        puts("error: operator not supported");
        exit(EXIT_FAILURE);
    }
}

/*
 * does something (pushing operands, popping the stack etc...) based on the current
 * content of the stack and the oper
 */
void action(Stack *s, char oper)
{
    Node *n = 0;

    // if oper is '(' then push it, a new set of operations must be performed
    // before the rest is written
    if(oper == '(')
    {
        push(s, oper);
    }
    // otherwise a closing bracket means that that set is done
    // then pop until the corresponding left bracket is found
    // (error if the stack is emptied before finding any)
    else if(oper == ')')
    {
        while(peek(*s) != '(')
        {
            if(peek(*s) == -1)
            {
                puts("error: mismatching brackets");
                exit(EXIT_FAILURE);
            }
            else
            {
                n = pop(s);
                printf("%c ", n->key);
                free(n);
            }
        }
        // then pop the '(' without writing it to stdout
        n = pop(s);
        free(n);
    }
    // otherwise is another operator: decide based on what's on the stack
    else
    {
        // if the stack is empty or has a left bracket, push the operator onto it
        if(peek(*s) == -1 || peek(*s) == '(')
        {
            push(s, oper);
        }
        // if the precedence of oper is higher than that at the top of the stack
        // push oper to the stack
        else if(precedence(peek(*s)) < precedence(oper))
        {
            push(s, oper);
        }
        else
        {
            // otherwise pop and print the top of the stack not empty and
            // until an operator
            // with lower or equal precedence is found or a left bracket
            while((*s) && (peek(*s) != '(') && (precedence(peek(*s)) >= precedence(oper)))
            {
                n = pop(s);
                printf("%c ", n->key);
                free(n);
            }
            // then push oper to the stack
            push(s, oper);
        }
    }
}

// arguments are provided via the array of strings **argv
int main(int argc, char **argv)
{
    if(argc < 2)
    {
        puts("No paramteters provided.\nUsage: ./infix-postfix \"<expr>\"\n expr MUST be quoted.");
        return 1;
    }

    // variables for parsing and the operator's stack
    int value;
    char operator;
    Stack opStack = NULL;

    char *token = (char*)malloc(TOK_SZ * sizeof(char));
    if(!token)
    {
        puts("Memory cannot be alloc'd");
        exit(EXIT_FAILURE);
    }

    // the argv[1] string is scanned
    token = strtok(argv[1], " ");
    while(token)
    {
        if(sscanf(token, "%d", &value) == 1)
        {
            // then an operand is read
            printf("%d ", value);
        }
        else
        {
            // otherwise argv[i] contains an operator
            sscanf(token, " %c", &operator);
            // then do something based on what's in the stack
            action(&opStack, operator);
        }

        // update token
        token = strtok(NULL, " ");
    }

    // token can be freed now
    free(token);

    // after all tokens have been read, then the operators on the stack must be popped
    // and written to output
    Node *n = 0;
    while(opStack)
    {
        n = pop(&opStack);
        // A left bracket must NOT be found in the final stack
        if(n->key == '(')
        {
            puts("error: mismatching brackets");
            exit(EXIT_FAILURE);
        }
        printf("%c ", n->key);
        free(n);
    }

    // then end with a newline
    putchar('\n');

    return 0;
}
