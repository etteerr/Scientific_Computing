#include "assignments.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <cstring>
#include <exception>

#define C 0.0
/*
    U_{i,j+1} = 
    \frac{C(\delta t)^2}{(\delta x)^2} %constant part
    \big[ U_{i+1,j} + U_{i-1,j} - 2U_{i,j} \big]
    + 2U_{i,j} - U_{i,j-1}
 * Forumla for vibrating string with:
 *  x as current position address (double *)
 *  px as current position in prev time (double *)
 *  dt: timestep in double
 *  dx: distance step in double
 * returns: nx (current position variable for next time step)
 */
#define Uij1(x, px, dt, dx) C * (pow(dt,2.0) / pow(dx, 2.0)) * ((*(x+1)) + (*(x-1)) - 2*(*x)) + 2*(*x) - *px

struct param {
    double dt = 0.001;
    double dx = 0.1;
    size_t segments = 10; //n or the amount of segments to simulate
    double time = 1.0; 
    int sfun = 0;
    char outputFile[256] = "vibratingString.jpg";
};

param process_arguments(int nargs, char ** args);

double f0(double x){return 0;}

double f1(double x){return sin(2*M_PI*x);}
double f2(double x){return sin(5*M_PI*x);}
double f3(double x){
    if(x>1.0/5.0 && x<2.0/5.0)
        return sin(5*M_PI*x);
    else
        return 0;
}


        

//Entry point
int vibratingString(int nargs, char** args) {
    param p = process_arguments(nargs, args);
    
    //Select init fun
    double(*fun)(double) = 0;
    
    switch (p.sfun) {
        case 0:
            fun = f0;
            break;
        case 1:
            fun = f1;
            break;
        case 2:
            fun = f2;
            break;
        case 3:
            fun = f3;
            break;
        default:
            fun = f0;
            break;
    }
    
    //Simulate and save
    double *arr = new double[p.segments];
    int i = 0;
    arr[i++] = 0;
    for(; i < p.segments-1;)
        arr[i++]=fun((double)i*p.dx);
    
    arr[i] = 0;
    
    double *parr = new double[p.segments];
    double *narr = new double[p.segments];
    
        
}


param process_arguments(int nargs, char ** args) {
    param p = param();
    int i = 0;
    
    if (nargs % 2 != 0) {
        vibratingString_help();
    }
    
    while(i < nargs) {
        char * cmd = args[i++];

        if (strcmp(cmd, "-n")==0)
            p.segments = atoll(args[i++]);

        if (strcmp(cmd, "-dt")==0)
            p.dt = atof(args[i++]);

        if (strcmp(cmd, "-dx")==0)
            p.dx = atof(args[i++]);

        if (strcmp(cmd, "-t")==0)
            p.time = atof(args[i++]);
        
        if (strcmp(cmd, "-f")==0)
            p.sfun = atoi(args[i++]);
        
        if (strcmp(cmd, "-o")==0)
            strcpy(p.outputFile, args[i++]);
    }
    
    //print output
    printf("Vibratring string:\n"
            "\t-dt [%f]\t specifies the timestep in seconds\n"
            "\t-dx [%f]\t specifies the distance between points in [cm]?\n"
            "\t-n  [%zi]\t specifies the amount of points to simulate\n"
            "\t-t  [%f]\t specifies the simulation time in seconds\n"
            "\t-f  [%i]\t specifies the initialization function [0-3]\n"
            "\t-o  [%s]\t specifies the output file, no ext appended!\n",
            p.dt, p.dx, p.segments, p.time, p.sfun, p.outputFile);
    
    return p;
}
//help
void vibratingString_help() {
    printf("Vibratring string help:\n"
            "\tSimulates a vibrating string.\n"
            "\t-dt [double] specifies the timestep in seconds\n"
            "\t-dx [double] specifies the distance between points in [cm]?\n"
            "\t-n  [size_t] specifies the amount of points to simulate\n"
            "\t-t  [double] specifies the simulation time in seconds\n"
            "\t-f  [int]    specifies the initialization function [0-3]\n"
            "\t-o  [string] specifies the output file, no ext appended!\n");
}