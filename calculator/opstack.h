#ifndef OPSTACK_H
#define OPSTACK_H

#define OPSTACK_SIZE 100

void op_push(char op);
int op_pop(char *op);
int op_peek(char *op);
int op_is_empty(void);
int op_is_full(void);

#endif