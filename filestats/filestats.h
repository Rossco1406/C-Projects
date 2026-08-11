#ifndef FILESTAT_H
#define FILESTAT_H

#include <stdio.h>

void filestats(FILE *fin, FILE *fout, const char *filein, const char *fileout);
void printStats(FILE *fin, FILE *fout, long fileSize, int lines, int words, int chars, const char *filein, const char *fileout);


#endif