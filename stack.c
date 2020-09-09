#include "stack.h"
#include <stdio.h>
#include <string.h>

// completes type definition
/* generic stack data structure, supports nodes of different types, as long as
 * the size is supplied at push
 */ 
struct node_t {
    void *key;
    struct node_t* next;
};
typedef struct node_t *Stack;

// pushes a string token of type tok_type to the stack, alloc done in the procedure
int push(Stack *s, void *token, size_t data_sz) {
    // alloc new node
    Stack head = (Stack) malloc(sizeof(struct node_t));
    // if the allocation didn't fail, then set the node to the top of the stack
    if(head) {
        // alloc token
        head->key = (void*)malloc(data_sz);
        if(head->key) {
            memcpy(head->key, token, data_sz);
            head->next = *s;
            *s = head;
            return 1;
        }
        else {
            puts("\nCannot alloc key");
        }
    }
    else {
        puts("\nCannot alloc node");
    }
    return 0;
}
// returns a copy of the node at the top of the stack, or NULL
void* peek(Stack s) {
    if(s) {
        return s->key;
    }
    else {
        return NULL;
    }
}
// pops the stack if there's at least an element
Stack pop(Stack *s) {
    if(*s) {
        Stack tmp = *s;
        void *k = (*s)->key;
        *s = (*s)->next;
        free(tmp);
        return k;
    }
    else {
        return NULL;
    }
}
