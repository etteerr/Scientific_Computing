#include "assignments.h"
#include <opencv2/opencv.hpp>


int test_opencv(int nargs, char ** args) {
    printf("Testing opencv...\n");
    
    printf("Creating image...\n");
    //Create image
    cv::Mat A(1080,1920, CV_8UC1);
    
    cv::randu(A, 0, 255);
    
    cv::imwrite("Testimage.jpg", A);
    
    printf("Saved image as Testimage.jpg\n");
    
    
    printf("Testing done.\n");
    
    return 0;
}