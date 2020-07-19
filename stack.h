// stack to hold operators

#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED

#include <stdlib.h>

typedef struct node_t {
	char key;
	struct node_t* next;
} Node;
typedef Node* Stack;

int push(Stack* s, const char token);
char peek(Stack s);
Node* pop(Stack* s);

#endif
