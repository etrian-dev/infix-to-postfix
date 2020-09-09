// stack to hold operators

#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED

#include <stdlib.h>

struct node_t;
typedef struct node_t *Stack;

int push(Stack *s, void *token, size_t data_sz);
void* peek(Stack s);
Stack pop(Stack *s);

#endif
