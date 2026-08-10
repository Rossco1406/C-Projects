#include <stdio.h>
#include "stack.h"

int main(void)
{
    push(10);
    push(20);
    push(30);

    printf("Top: %.2f\n", peek());

    printf("Pop: %.2f\n", pop());
    printf("Pop: %.2f\n", pop());
    printf("Pop: %.2f\n", pop());

    printf("Empty: %d\n", is_empty());

    return 0;
}