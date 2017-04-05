#include "assignments.h"
#include <stdio.h>
#include <stdlib.h>
#include <opencv2/opencv.hpp>


//Entry point
int assignment1(int nargs, char** args) {
    printf("Arguments received: %i\n", nargs);
    if (nargs < 1 ) return 0;
    int a = atoi(args[0]);
    printf("Atoi gives %i\n", a);
    
    //Create image
    cv::Mat A(1920,1080, CV_8UC1);
    
    cv::randu(A, 0, 255);
    
    cv::imwrite("Testimage.jpg", A);
    //cvSaveImage("test.jpg", A);
    
    return 0;
}

//help
void ass1_help() {
    printf("Example help.\n");
}