#include "assignments.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <cstring>
#include <exception>
#include <opencv2/opencv.hpp>

#define C 1.0
#define w 512

using namespace cv;

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
    double dt = 0.01;
    double dx = 0.01;
    size_t segments = 100; //n or the amount of segments to simulate
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
     
//void MyLine( Mat img, Point start, Point end ){
  //int thickness = 2;
  //int lineType = LINE_8;
 // line( img,
 //   start,
 //   end,
 //   Scalar( 255, 255, 255 ),
  //  thickness,
  //  lineType );
//}

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
    
    //Initialization
    double *arr = new double[p.segments];
    double *parr = new double[p.segments];
    double *narr = new double[p.segments];
    double *tmp = new double[p.segments];
    int *xpoints = new int[p.segments];
    int timesteps = p.time/p.dt;
    int i = 0;
//    int w,h;
    
    arr[i] = 0;
    parr[i] = 0;
    narr[i] = 0;
    xpoints[i] = 0;
    for(i=1; i < p.segments-1;i++){
        arr[i] = fun((double)i*p.dx);
        parr[i] = fun((double)i*p.dx);
        narr[i] = fun((double)i*p.dx);
        xpoints[i] = ((double)i/p.segments)*w;
    }
    
    arr[++i] = 0;
    parr[i] = 0;
    narr[i] = 0;
    xpoints[i] = w;
    int last =  p.segments - 1;
    int thickness = 2;
    int lineType = LINE_8;
    VideoWriter outp;
    Size s = Size(w,w);
    outp.open("video.avi", CV_FOURCC('h','2','6','4'), 30, s, 1);
    Mat image = Mat::eye(w,w, CV_8UC3);
 
    //Simulate and save
    for(int j=0; j < timesteps; j++){
        
        for(int k=1; k < p.segments-1; k++){
            narr[k] = Uij1(&arr[k], &parr[k], p.dt, p.dx);
            line(image, Point(xpoints[k-1], (int)(arr[k-1]+w/2)), Point(xpoints[k], (int)(arr[k]+w/2)), Scalar( 255, 255, 255 ), thickness, lineType );
            //MyLine(image, Point(xpoints[k-1], arr[k-1]+w/2),Point(xpoints[k], arr[k]+w/2) );
        }
        line( image, Point(xpoints[last-1], (int)(arr[last-1]+w/2)),Point(xpoints[last],(int)(arr[last]+w/2)), Scalar( 255, 255, 255 ), thickness, lineType );
        //MyLine(image, Point(xpoints[last-1], arr[last-1]+w/2),Point(xpoints[last],arr[last]+w/2));
        outp << image;
        
        tmp = parr;
        parr = arr;
        arr = narr; 
        narr = tmp;
    }

    
    
    
        
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