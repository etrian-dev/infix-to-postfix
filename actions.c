#include "stack.h"
#include <stdio.h>

/* determines precedence of operator op:
 * {+, -} = 0
 * {*, /, %} = 1
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
 * does something (pushing operands, popping the stack etc...) based on the current
 * content of the stack and the oper
 */
void action(Stack *stack, const char new_operator)
{
    char *popped = NULL;

    // if oper is '(' then push it, a new set of operations must be performed
    // before the rest is written
    if(new_operator == '(')
    {
        push(stack, (void*)&new_operator, sizeof(char));
    }
    // otherwise a closing bracket means that that set is done
    // then pop until the corresponding left bracket is found
    // (error if the stack is emptied before finding any)
    else if(new_operator == ')')
    {
        while(*(char*)peek(*stack) != '(')
        {
            // if the stack is empty, then there is an error in the input
            if(peek(*stack) == NULL)
            {
                puts("error: mismatching brackets");
                exit(EXIT_FAILURE);
            }
            // otherwise pop it and print it
            else
            {
                popped = (char *)pop(stack);
                printf("%c ", *popped);
                free(popped);
            }
        }
        // then pop the '(' without writing it to stdout
        popped = (char *)pop(stack);
        free(popped);
    }
    /*
     * if another operator is read (other than ')':
     * decide based on what's on the stack
     */
    else
    {
        // if the stack is empty or has a left bracket, push any operator onto it
        if(peek(*stack) == NULL || *(char *)peek(*stack) == '(')
        {
            push(stack, (void*)&new_operator, sizeof(char));
        }
        // if the precedence of oper is higher than that at the top of the stack
        // push oper to the stack
        else if(precedence(*(char *)peek(*stack)) < precedence(new_operator))
        {
            push(stack, (void*)&new_operator, sizeof(char));
        }
        else
        {
            // otherwise pop and print the top of the stack not empty and
            // until an operator
            // with lower or equal precedence is found or a left bracket
            while(
                (*stack)
                && (*(char *)peek(*stack) != '(')
                && (precedence(*(char *)peek(*stack)) >= precedence(new_operator)))
            {
                popped = (char *)pop(stack);
                printf("%c ", *popped);
                free(popped);
            }
            // then push oper to the stack
            push(stack, (void*)&new_operator, sizeof(char));
        }
    }
}
