#include "assignment1.h"
#include <stdio.h>
#include <stdlib.h>

//Entry point
int assignment1(int nargs, char** args) {
    printf("Arguments received: %i\n", nargs);
    if (nargs < 1 ) return 0;
    int a = atoi(args[0]);
    printf("Atoi gives %i\n", a);
    return 0;
}

//help
void ass1_help() {
    printf("Example help.\n");
}