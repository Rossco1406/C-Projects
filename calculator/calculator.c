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
        if (c == '\n') {
            int success = 1;
            while (!op_is_empty()){
                if (!calculate_top()) {
                    success = 0;
                    break;
                }
            }
        
            if (success && !is_empty()) {
                double result;
                
                if (peek(&result))
                    printf("Result: %.2f\n", result);
            }

            clear_stack();
            op_clear();

            continue;
        }

        ungetch(c);

        if (get_number(&value)) {
            push(value);
            continue;
        }

        c = getch();

        if (c == '+' || c == '-' || c == '*' || c == '/') {

            while (!op_is_empty()){
                char top;

                op_peek(&top);
                
                if (precedence(top) < precedence(c))
                    break;
                
                calculate_top();
            }
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

int calculate_top(void){
    double operand_1, operand_2;
    char operator;
    double result;

    if (!op_pop(&operator))
        return 0;

    if (!pop(&operand_2))
        return 0;

    if (!pop(&operand_1))
        return 0;

    if (!apply_operator(operator, operand_1, operand_2, &result))
        return 0;

    push(result);

    return 1;
}