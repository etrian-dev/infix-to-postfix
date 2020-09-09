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
#include <ctype.h>
#include <errno.h>

void action(Stack *stack, char new_operator);

// define a long enough token size
#define MAX_LEN_DIGIT 51

// arguments are provided via the array of strings **argv
int main(int argc, char **argv)
{
    if(argc < 2)
    {
        puts("No paramteters provided.\nUsage: ./infix-postfix \"<expr>\"\n expr MUST be quoted.");
        return 1;
    }
    // read the string in argv[1] one character at a time and process it
    else
    {
        errno = 0; // for range error in string conversion

        int i = 0;
        char c[2] = {' ', '\0'};
        char *value = (char*)calloc(MAX_LEN_DIGIT, sizeof(char));
        long int flag_conv;
        int num_len = 0; // this counter maintains the digit count in *value

        Stack opStack = NULL; // this stack holds the operators

        while(argv[1][i])
        {
            c[0] = argv[1][i];
            // if it's a digit, then concatenate to the value string
            if(isdigit(c[0]))
            {
                if(num_len == MAX_LEN_DIGIT - 1)
                {
                    puts("\nInput error: number too long");
                    exit(EXIT_FAILURE);
                }
                strcat(value, c); // concatenate c to value str
                num_len++;
            }
            else
            {
                if(value[0] != '\0') {
                // first of all convert the string to a value and then reset it
                    flag_conv = strtol(value, NULL, 10);
                    memset(value, 0, MAX_LEN_DIGIT);
                    num_len = 0; // reset num

                    // if the conversion overflows, then notify and abort
                    if(errno == ERANGE)
                    {
                        puts("\nConversion error: overflow");
                        exit(EXIT_FAILURE);
                    }

                    printf("%ld ", flag_conv);
                }

                // skips blanks (when isblank returns 0
                if(isblank(c[0]) == 0)
                {
                    // otherwise it's an operator and an action must be performed
                    action(&opStack, c[0]);
                }
            }
            
            i++;
        }
        
        //if there is a value left in the string value, that needs to be written to output
        if(value[0] != '\0') {
            flag_conv = strtol(value, NULL, 10);
            memset(value, 0, MAX_LEN_DIGIT);
            num_len = 0; // reset num

            // if the conversion overflows, then notify and abort
            if(errno == ERANGE)
            {
                puts("\nConversion error: overflow");
                exit(EXIT_FAILURE);
            }

            printf("%ld ", flag_conv);
        }

        /*
         *  after all tokens have been read, then the operators left
         * on the stack must be removed and written to output
         */
        char *oper = NULL;
        while(opStack)
        {
            oper = (char *)pop(&opStack);
            // A left bracket must NOT be found in the final stack
            if(*oper == '(')
            {
                puts("error: mismatching brackets");
                exit(EXIT_FAILURE);
            }
            printf("%c ", *oper);
            free(oper);
        }

        // then end with a newline
        putchar('\n');
    }

    return 0;
}
