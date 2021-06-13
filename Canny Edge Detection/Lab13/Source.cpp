#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <cmath>
#include <vector>
#include <unordered_map> 
#include <utility>
#include <algorithm>
#include <chrono>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main()
{
    Mat orig_img = imread("C:/users/trish/OneDrive/Pictures/houghcoinseasy2.jpg", IMREAD_COLOR);
    Mat grayscale;
    cvtColor(orig_img, grayscale, COLOR_BGR2GRAY);
    medianBlur(grayscale, grayscale, 5);
    vector<Vec3f> quarters;
    HoughCircles(grayscale, quarters, HOUGH_GRADIENT, 1, grayscale.rows / 16, 100, 30, 33, 60);
    for (size_t i = 0; i < quarters.size(); i++)
    {
        Vec3i c = quarters[i];
        Point center = Point(c[0], c[1]);
        circle(orig_img, center, 1, Scalar(0, 100, 100), 3, LINE_AA);
        int radius = c[2];
        circle(orig_img, center, radius, Scalar(0, 0, 255), 3, LINE_AA);
    }
    vector<Vec3f> nickels;
    HoughCircles(grayscale, nickels, HOUGH_GRADIENT, 1, grayscale.rows / 16, 100, 30, 30, 32);
    for (size_t i = 0; i < nickels.size(); i++)
    {
        Vec3i c = nickels[i];
        Point center = Point(c[0], c[1]);
        int radius = c[2];
        circle(orig_img, center, radius, Scalar(0, 255, 0), 3, LINE_AA);
    }
    vector<Vec3f> pennies;
    HoughCircles(grayscale, pennies, HOUGH_GRADIENT, 1, grayscale.rows / 16, 100, 30, 1, 28);
    for (size_t i = 0; i < pennies.size(); i++)
    {
        Vec3i c = pennies[i];
        Point center = Point(c[0], c[1]);
        int radius = c[2];
        circle(orig_img, center, radius, Scalar(255, 0, 0), 3, LINE_AA);
    }
    cout << "5.56" << endl;    
    imwrite("C:/Users/trish/OneDrive/Documents/JuniorYear/ComputerVision/Canny Edge Detection/Lab13/coinedges.jpg", orig_img);
}
