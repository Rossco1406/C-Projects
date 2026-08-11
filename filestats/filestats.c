#include "filestats.h"
#include <ctype.h>

void filestats(FILE *fin, FILE *fout){

    int c;
    int lines = 0;
    int words = 0;
    int chars = 0;
    int inWord = 0;
    int last = '\0';

    /* Get FileSize */
    fseek(fin, 0, SEEK_END);
    long fileSize = ftell(fin);
    fseek(fin, 0, SEEK_SET);

    while ((c = fgetc(fin)) != EOF){

        /* Get Number of Lines*/
        if (c == '\n')
            lines++;

        /* Get Number of words*/
        if (isspace(c)) {
            inWord = 0;
        } else if (inWord == 0) {
            words++;
            inWord = 1;
        }


        /* Get number of characters*/ 
        chars++;

        last = c;
    }

    if (last != '\n' && chars > 0)
        lines++;
}

void printStats(FILE *fin, FILE *fout, long fileSize, int lines, int words, int chars, const char *namein, const char *nameout){

    if (namein != NULL)
        fprintf(fout, "Filename: %s \n", namein);
    else
        fprintf(fout, "Keyboard input\n");

    fprintf(fout, "File Size: %ld \n",fileSize);
    fprintf(fout, "Lines: %d \n", lines);
    fprintf(fout, "Words: %d \n", words);
    fprintf(fout, "Characters: %d \n", chars);

    if (nameout != NULL)
        fprintf(stderr, "Statistics written to %s\n", nameout);
    else
        fprintf(stderr, "Statistics written to Terminal\n");
}
