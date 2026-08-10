#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#define BUFSIZE 100

static char buf[BUFSIZE];
static int bufp;

int getch(void)
{
    if (bufp > 0)
        return buf[--bufp];

    return getchar();
}

void ungetch(int c)
{
    if (bufp < BUFSIZE)
        buf[bufp++] = c;
    else
        printf("Error: too many characters pushed back\n");
}

int get_number(double *value)
{
    int c;
    char number[BUFSIZE];
    int i = 0;
    int digits = 0;

    while (isspace(c = getch()))
        ;

    if (!isdigit(c) && c != '.') {
        ungetch(c);
        return 0;
    }

    while (isdigit(c)) {
        number[i++] = c;
        digits = 1;
        c = getch();
    }

    if (c == '.') {
        number[i++] = c;
        c = getch();
    }

    while (isdigit(c)) {
        number[i++] = c;
        digits = 1;
        c = getch();
    }

    if (!digits) {
        printf("Error: Invalid number\n");
        ungetch(c);
        return 0;
    }

    number[i] = '\0';

    ungetch(c);

    *value = strtod(number, NULL);

    return 1;
}