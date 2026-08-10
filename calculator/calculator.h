#ifndef CALC_H
#define CALC_H

enum precedence_level {
    PREC_NONE,
    PREC_ADD_SUB,
    PREC_MULT_DIV,
    PREC_POWER,
    PREC_PARENTH
};

void calculator(void);
int apply_operator(char op, double a, double b, double *result);
enum precedence_level precedence(char op);
int calculate_top(void);
int calculate_parenthesis(void);
#endif