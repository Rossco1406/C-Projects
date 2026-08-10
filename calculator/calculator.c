#include "calculator.h"
#include "stack.h"
#include "opstack.h"
#include "input.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>


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

void calculator(void){
    int c;
    double value;

    while ((c = getch()) != EOF) {
        ungetch(c);

        if (get_number(&value)) {
            printf("NUMBER: %.2f\n", value);
            push(value);
            continue;
        }

        c = getch();

        if (c == '+' || c == '-' || c == '*' || c == '/') {
            printf("OPERATOR: %c\n", c);
            op_push(c);
        }

        else if (!isspace(c)) {
            printf("Error: Unknown character '%c'\n", c);
        }
    }
}

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