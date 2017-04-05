/* 
 * File:   main.c
 * Author: Erwin Diepgrond <e.j.diepgrond@gmail.com>
 *
 * Created on April 3, 2017, 10:12 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "assignments.h"

struct program_entry {
    const char name[256];
    int (*prog)(int , char**);
    void (*args)();
};

const struct program_entry program_entries[] = {
    {"vibratingString", vibratingString, vibratingString_help},
    {"testOpenCV", test_opencv, 0}
};


void print_specific(char * name) {
    unsigned int nEntries = sizeof(program_entries)/sizeof(struct program_entry);
    char help_printed = 0;
    
    for(unsigned int i = 0; i < nEntries; i++)
        if ( strcmp(program_entries[i].name, name)==0) {
            if (program_entries[i].args) {
                program_entries[i].args();
                help_printed = 1;
            }
            break;
        }
    
    if (!help_printed) printf("No help specified for %s.\n", name);            
}

void print_options() {
    unsigned int nEntries = sizeof(program_entries)/sizeof(struct program_entry);
    
    printf("Usage: ./scientific_computing [program] [program arguments]\n");
    printf("Usage: ./scientific_computing help [program]\n");
    printf("Options are: \n");
    
    for(unsigned int i = 0; i < nEntries; i++)
        printf("\t%s\n", program_entries[i].name);
    printf("For more options: help [name of option]\n");
}
/*
 * Main program entry point
 *  Selects the right program to run by the arguments given.
 *  Prints possible arguments
 */
int main(int nargs, char** args) {
    
    if (nargs < 2) {
        printf("No arguments given.\n");
        print_options();
        return 0;
    }
    
    int arg = 1;
    
    if (strcmp(args[arg], "help")==0) {
        if (nargs < 3) {
            print_options();
            return 0;
        }
        print_specific(args[++arg]);
        return 0;
    }
    
    unsigned int nEntries = sizeof(program_entries)/sizeof(struct program_entry);
    
    unsigned int i = 0;
    for(; i<nEntries; i++) {
        if (strcmp(program_entries[i].name, args[arg])==0) {
            break;
        }
    }
    
    return program_entries[i].prog(nargs - 2, &args[++arg]);
}

