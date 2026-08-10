#include "calculator.h"
#include "stack.h"
#include "opstack.h"
#include <math.h>
#include <stdio.h>

enum precedence_level precedence(char op)
{
    switch (op) {
        case '+':
        case '-':
            return PREC_ADD_SUB;

        case '*':
        case '/':
            return PREC_MULT_DIV;

        default:
            return PREC_NONE;
    }
}

void calculator(void){}

int apply_operator(char op, double a, double b, double *result){
    switch (op){
        case '+':
            *result = a + b;
            return 1;

        case '-':
            *result = a - b;
            return 1;

        case '*':
            *result = a * b;
            return 1;

        case '/':
            if (b == 0) {
                printf("Error: Cannot divide by 0\n");
                return 0;
            }

            *result = a / b;
            return 1;

        default:
            printf("Error: Unknown operator\n");
            return 0;
    }
}

