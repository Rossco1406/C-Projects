#include "stack.h"
#include <stdio.h>

static double stack[STACK_SIZE];
static int sp;

void push(double value){
    if (is_full() == 0)
        stack[sp++] = value;
    else
        printf("Error: Stack is full, cannot push\n");
}

int pop(double *value){
    double value;

    if (is_empty() == 1){
        printf("Error: Cannot pop, stack is empty\n");
        return 0;
    } else {
        *value = stack[--sp];
        return 1;
    }
}

int peek(double *value){
    double value;

    if (is_empty() == 1){
        printf("Error: Cannot peek, stack is empty\n");
        return 0;
    } else{
        *value = stack[sp-1];
        return 1;
    }
}

int is_empty(void){
    if (sp == 0)
        return 1;
    return 0;
}

int is_full(void){
    if (sp == STACK_SIZE)
        return 1;
    return 0;
}