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

// define a long enough token size to accomodate big numbers
#define MAX_LEN_DIGIT 51

// arguments are provided via the array of strings **argv
int main(int argc, char **argv)
{
    if(argc != 2)
    {
        puts("No parameters provided.\nUsage: ./infix-postfix \"<expr>\"\n expr MUST be quoted.");
        return 1;
    }
    // read the string in argv[1] one character at a time and process it
    else
    {
      // this string stores the digits of the current number being read
      char *value = (char*)calloc(MAX_LEN_DIGIT, sizeof(char));
      int num_len = 0; // this counter stores the digit count
      long int long_of_value; // the converted value is stored in this variable
      //useful char array to use functions operating on strings (string.h)
      char c[2] = {' ', '\0'};

      errno = 0; // for range errors in string conversion
      int i = 0;

      Stack opStack = NULL; // this stack holds the operators

        // The loop processes every character in the input expression
        while(argv[1][i])
        {
            // update the string with the current char
            c[0] = argv[1][i];

            // if it's a digit, then concatenate it to the value string
            if(isdigit(c[0]))
            {
                if(num_len == MAX_LEN_DIGIT - 1)
                {
                    puts("\nInput error: too many digits");
                    exit(EXIT_FAILURE);
                }
                strcat(value, c); // concatenate c to value
                num_len++; // increase the length
            }
            else
            {
                // if there was some value stored in the value buffer, convert it
                if(value[0] != '\0') {
                    // conversion uses strtol
                    long_of_value = strtol(value, NULL, 10);
                    memset(value, 0, MAX_LEN_DIGIT); // resets to 0 the buffer
                    num_len = 0; // resets number lenght as well

                    // if the conversion overflows or underflows, then notify and abort
                    if(errno == ERANGE)
                    {
                        puts("\nConversion error: overflow");
                        exit(EXIT_FAILURE);
                    }
                    // otherwise everything is fine: print the number to stdout
                    printf("%ld ", long_of_value);
                }

                // skips blanks (detected if isblank returns 1)
                if(isblank(c[0]) == 0)
                {
                    // otherwise it's an operator and an action
                    // must be performed on it
                    action(&opStack, c[0]);
                }
            }

            // then update the index to get the next character in the input
            i++;
        }

        // a quick check: the last character must be either a digit or a ')'
        // otherwise the expression is malformed
        if(!isdigit(argv[1][i-1]) && argv[1][i-1] != ')') {
          puts("\nInput error: an expression must be terminated by either a digit or a \')\'");
          exit(EXIT_FAILURE);
        }

        //if there is value yet to be converted (the expr ends with a number)
        // then that needs to be written to output before emptying the stack
        // the instructions are the same as before
        if(value[0] != '\0') {
            long_of_value = strtol(value, NULL, 10);
            memset(value, 0, MAX_LEN_DIGIT);
            num_len = 0;
            if(errno == ERANGE)
            {
                puts("\nConversion error: overflow");
                exit(EXIT_FAILURE);
            }
            printf("%ld ", long_of_value);
        }

        /*
         * After all tokens have been read, then the operators left
         * on the stack must be popped and written to output
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

        // then end the output with a newline
        putchar('\n');
    }

    return 0;
}
