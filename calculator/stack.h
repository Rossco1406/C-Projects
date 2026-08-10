#ifndef STACK_H
#define STACK_H

#define STACK_SIZE 100

void push(double value);
int pop(double *value);
int peek(double *value);
int is_empty(void);
int is_full(void);
void clear_stack(void);

#endif