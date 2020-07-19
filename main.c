// program to convert any infix expression to postfix
// following a particular format
// [format specs here]

// include stack library
#include "stack.h"
// include readline
#include <readline/readline.h> 
// include string.h

// define tokens size (big enough hopefully)
#define TOK_SZ 50

// determine action based on operator and contents of the stack
void action(Stack *s, char oper);
/* determines precedence of operator op:
 * {+, -} = 0
 * {*, /} = 1;
 */
int precedence(char op);

int main(void) {
	// variables to hold input line and tokens respectively
	char *line = 0, *token = 0;
	
	// read input line
	//line = readline("insert expression: ");
	line = readline(NULL);
	
	// alloc token
	token = (char*) calloc(TOK_SZ, sizeof(*token));
	
	// if alloc is fine, then process line
	if(line && token) {
		// store line lenght and current position in it, starting by 0
		//size_t line_len = strlen(line), cur_len = 0;
		// values used for token processing
		int value; char operator;
		// the stack (holds operators)
		Stack opStack = 0;
		
		// tokenize the input
		token = strtok(line, " ");
		while(token) {
			
			// process current token: if it's an operand, output it
			if(sscanf(token, "%d", &value) == 1) {
				// the format specifies that it must be followed by a space
				printf("%d ", value);
			}
			// otherwise it's an operator
			else {
				// first parse it as a single char
				sscanf(token, " %c", &operator);
				// then determine the action based on some rules
				// and the content of the stack
				action(&opStack, operator);
			}
			
			// update token
			token = strtok(NULL, " ");
		}
		
		// free line
		free(line);
		free(token);
		
		// after the line is processed the stack must be popped until it's empty
		// and the operands will be written as they are popped
		Node *n = 0;
		while(opStack) {
			n = pop(&opStack);
			// left bracket must NOT be found in the final stack
			if(n->key == '(') {
				puts("error: mismatching brackets");
				exit(EXIT_FAILURE);
			}
			printf("%c ", n->key);
			free(n);
		}
		// then end with a newline
		putchar('\n');
	}
}

void action(Stack *s, char oper) {
	Node *n = 0;
	
	// if oper is '(' then push it, a new set of operations must be performed
	// before the rest is written
	if(oper == '(') {
		push(s, oper);
	}
	// otherwise a closing bracket means that that set is done
	// then pop until the corresponding left bracket is found 
	// (error if the stack is emptied before finding any)
	else if(oper == ')') {
		while(peek(*s) != '(') {
			if(peek(*s) == -1) {
				puts("error: mismatching brackets");
				exit(EXIT_FAILURE);
			}
			else {
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
	else {
		// if the stack is empty or has a left bracket, push the operator onto it
		if(peek(*s) == -1 || peek(*s) == '(') {
			push(s, oper);
		}
		// if the precedence of oper is higher than that at the top of the stack
		// push oper to the stack
		else if(precedence(peek(*s)) < precedence(oper)) {
			push(s, oper);
		}
		else {
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
/* determines precedence of operator op:
 * {+, -} = 0
 * {*, /} = 1
 */
int precedence(char op) {
	switch(op) {
		case '+': case '-': 
			return 0;
		case '*': case '/':
			return 1;
		default:
			puts("error: operator not supported");
			exit(EXIT_FAILURE);
	}
}
