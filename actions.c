// my header: a generic stack using void pointers
#include "stack.h"

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

/*
 * Determines precedence of operator op
 * {+, -} = 0
 * {*, /, %} = 1
 * {^} = 2
 */
int precedence(const char op)
{
    switch(op)
    {
    case '+':
    case '-':
        return 0;
    case '*':
    case '/':
    case '%':
        return 1;
    case '^':
        return 2;
    default:
        puts("\nInput error: operator not supported");
        exit(EXIT_FAILURE);
    }
}

/*
 * Executes an action (pushing operands, popping the stack etc...)
 * based on the current content of the stack and the oper
 */
void action(Stack *stack, const char oper)
{
    char *popped = NULL;

    // If oper is '(' then push it to the stack
    // because expression between (...) are evaluated before the current level
    if(oper == '(')
    {
        push(stack, (void*)&oper, sizeof(char));
    }

    // A closing bracket means that that the subexpression terminates, so its
    // operators are all on the stack currently
    // So pop it until the corresponding left bracket is found
    // Obiviously errors can occur and are treated with an error message & exit()
    else if(oper == ')')
    {
        // until the top is not '('
        while(*(char*)peek(*stack) != '(')
        {
            // stack empty -> brackets mismatch in the input string -> error
            if(peek(*stack) == NULL)
            {
                puts("error: mismatching brackets");
                exit(EXIT_FAILURE);
            }
            // otherwise pop the operator and print it
            else
            {
                popped = (char *)pop(stack);
                printf("%c ", *popped);
                free(popped); // then free its memory
                popped = NULL;
            }
        }
        // All operators have been written: pop '(' but don't output it
        popped = (char *)pop(stack);
        free(popped);
    }
    /*
     * If another operator is read then
     * the action is decided based on what's on the stack
     */
    else
    {
        // if the stack is empty or has a opening bracket, push any operator onto it
        if(peek(*stack) == NULL || *(char *)peek(*stack) == '(')
        {
            push(stack, (void*)&oper, sizeof(char));
        }
        // Otherwise, if the precedence of oper is higher than the that of
        // the symbol at the top of the stack -> push oper to the stack
        else if(precedence(oper) > precedence(*(char *)peek(*stack)))
        {
            push(stack, (void*)&oper, sizeof(char));
        }
        else
        {
            // Otherwise pop and print the top of the stack until an operator
            // with lower or equal precedence is found (or a left bracket)
            while(
                (*stack)
                && (*(char *)peek(*stack) != '(')
                && (precedence(*(char *)peek(*stack)) >= precedence(oper)))
            {
                popped = (char *)pop(stack);
                printf("%c ", *popped);
                free(popped);
            }
            // then the top of the stack has precedence < than oper
            // so push oper to the stack
            push(stack, (void*)&oper, sizeof(char));
        }
    }
}
