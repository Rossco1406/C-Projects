#include <stdio.h>
#include "stack.h"

#include <stdio.h>
#include "calculator.h"

int main(void)
{
    double result;

    if (apply_operator('+', 2, 5, &result))
        printf("2 + 5 = %.2f\n", result);

    if (apply_operator('-', 10, 4, &result))
        printf("10 - 4 = %.2f\n", result);

    if (apply_operator('*', 3, 7, &result))
        printf("3 * 7 = %.2f\n", result);

    if (apply_operator('/', 20, 5, &result))
        printf("20 / 5 = %.2f\n", result);

    if (apply_operator('/', 20, 0, &result))
        printf("20 / 0 = %.2f\n", result);

    if (apply_operator('?', 20, 5, &result))
        printf("20 ? 5 = %.2f\n", result);

    return 0;
}