#include "filestats.h"

int main(int argc, char *argv[])
{   
    FILE *input;
    FILE *output;
    char *prog = argv[0];

    if (argc == 1)
        filestats(stdin,stdout,NULL,NULL);
    else if (argc == 2){
        input = fopen(argv[1], "r");
        if (input == NULL) {
            fprintf(stderr, "%s:  %s doesnt exist\n", prog, argv[1]);
            return 1;
        }
        filestats(input,stdout,argv[1],NULL);
        fclose(input);

    }
    else if (argc == 3){
        input = fopen(argv[1], "r");

        if (input == NULL) {
            fprintf(stderr, "%s: cannot open %s for reading\n", prog, argv[1]);
            return 1;
        }

        output = fopen(argv[2], "w");

        if (output == NULL){
            fprintf(stderr, "%s: cannot open %s for writing\n", prog, argv[2]);
            fclose(input);
            return 1;
        }
        
        filestats(input, output, argv[1],argv[2]);

        fclose(input);
        fclose(output);
    }
    else {
        printf("Usage: %s [input] [output]\n", argv[0]);
        return 1;
    }

    return 0;
}