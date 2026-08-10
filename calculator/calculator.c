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
        case '^':
            return PREC_POWER;

        default:
            return PREC_NONE;
    }
}

void calculator(void){
    int c;
    double value;
    int error = 0;

    while ((c = getch()) != EOF) {
        
        if (c == '\n') {
            if (!error) {
                while (!op_is_empty()){
                    char top;

                    op_peek(&top);

                    if (top == '(') { 
                        printf("Error: Missing \')\'\n");
                        error = 1;
                        break;
                    }

                    if (!calculate_top()) { 
                        error = 1; 
                        break; 
                    }
                }
            }
        
            if (!error && !is_empty()) {
                double result;
                
                if (peek(&result))
                    printf("Result: %.2f\n", result);
            }

            clear_stack();
            op_clear();
            error = 0;

            continue;
        }

        ungetch(c);

        if (get_number(&value)) {
            push(value);
            continue;
        }

        c = getch();

        if (c == '(') {
            op_push(c);
        }
        else if (c == ')') {
            if (!calculate_parenthesis())
                error = 1;
        }   
        else if (c == '+' || c == '-' || c == '*' || c == '/' || '^') {

            while (!op_is_empty()){
                char top;

                op_peek(&top);

                if (top == '(')
                    break;
                
                if (precedence(top) < precedence(c))
                    break;

                if (c == '^' && top == '^')
                    break;
                
                if (!calculate_top()){
                    error = 1; 
                    break;
                }
            }
            if (!error)
                op_push(c);
        }

        else if (!isspace(c)) {
            printf("Error: Unknown character '%c'\n", c);
            error = 1;
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
        case '^':
            *result = pow(a, b);
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

int calculate_parenthesis(void)
{
    char top;

    while (!op_is_empty()) {

        op_peek(&top);

        if (top == '(') {
            op_pop(&top);
            return 1;
        }

        if (!calculate_top())
            return 0;
    }

    printf("Error: Missing '('\n");
    return 0;
}