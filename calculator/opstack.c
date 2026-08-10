#include "opstack.h"
#include <stdio.h>


static char stack[OPSTACK_SIZE];
static int sp;

void op_push(char op){
    if (op_is_full() == 0)
        stack[sp++] = op;
    else
        printf("Error: Stack is full, cannot push\n");
}

int op_pop(char *op){

    if (op_is_empty() == 1){
        printf("Error: Cannot pop, stack is empty");
        return 0;
    } else {
        *op = stack[--sp];
        return 1;
    }
}

int op_peek(char *op){

    if (op_is_empty() == 1){
        printf("Error: Cannot peek, stack is empty");
        return 0;
    } else{
        *op = stack[sp-1];
        return 1;
    }
}

int op_is_empty(void){
    if (sp == 0)
        return 1;
    return 0;
}

int op_is_full(void){
    if (sp == OPSTACK_SIZE)
        return 1;
    return 0;
}

void op_clear(void)
{
    sp = 0;
}