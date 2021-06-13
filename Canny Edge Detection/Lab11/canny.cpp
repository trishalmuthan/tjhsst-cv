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
    //READ IN IMAGE
    Mat orig_img = imread("C:/users/trish/OneDrive/Pictures/monarch.jpg", IMREAD_COLOR);
    imshow("Original image", orig_img);
    int k = waitKey(0);

    //MAKE GRAYSCALE IMAGE
    vector<vector<int>> gs_vec(orig_img.rows, vector<int>(orig_img.cols, 0));
    //Mat gs_img(orig_img.rows, orig_img.cols, CV_8U);
    for (int i = 0; i < orig_img.rows; i++)
    {
        for (int j = 0; j < orig_img.cols; j++)
        {
            // B G R
            int blue = orig_img.at<Vec3b>(i, j)[0];
            int green = orig_img.at<Vec3b>(i, j)[1];
            int red = orig_img.at<Vec3b>(i, j)[2];
            int grayscale_val = (int)((blue + green + red) / 3.0);
            gs_vec[i][j] = grayscale_val;
        }
    }

    //MAKE gX VECTOR
    vector<vector<int>> gX(orig_img.rows, vector<int>(orig_img.cols, 0));
    for (int i = 0; i < orig_img.rows; i++)
    {
        for (int j = 0; j < orig_img.cols; j++)
        {
            int sum = 0;
            //top-left
            if (i - 1 >= 0 && j - 1 >= 0)
            {
                sum += (-1 * gs_vec[i - 1][j - 1]);
            }
            //top-middle
            if (i - 1 >= 0)
            {
                sum += (0 * gs_vec[i - 1][j]);
            }
            //top-right
            if (i - 1 >= 0 && j + 1 < orig_img.cols)
            {
                sum += (1 * gs_vec[i - 1][j + 1]);
            }
            //left
            if (j - 1 >= 0)
            {
                sum += (-2 * gs_vec[i][j - 1]);
            }
            //center
            sum += (0 * gs_vec[i][j]);
            //right
            if (j + 1 < orig_img.cols)
            {
                sum += (2 * gs_vec[i][j + 1]);
            }
            //bottom-left
            if (i + 1 < orig_img.rows && j - 1 >= 0)
            {
                sum += (-1 * gs_vec[i + 1][j - 1]);
            }
            //bottom-middle
            if (i + 1 < orig_img.rows)
            {
                sum += (0 * gs_vec[i + 1][j]);
            }
            //bottom-right
            if (i + 1 < orig_img.rows && j + 1 < orig_img.cols)
            {
                sum += (1 * gs_vec[i + 1][j + 1]);
            }
            //insert value
            gX[i][j] = sum;
        }
    }

    //MAKE gY VECTOR
    vector<vector<int>> gY(orig_img.rows, vector<int>(orig_img.cols, 0));
    for (int i = 0; i < orig_img.rows; i++)
    {
        for (int j = 0; j < orig_img.cols; j++)
        {
            int sum = 0;
            //top-left
            if (i - 1 >= 0 && j - 1 >= 0)
            {
                sum += (1 * gs_vec[i - 1][j - 1]);
            }
            //top-middle
            if (i - 1 >= 0)
            {
                sum += (2 * gs_vec[i - 1][j]);
            }
            //top-right
            if (i - 1 >= 0 && j + 1 < orig_img.cols)
            {
                sum += (1 * gs_vec[i - 1][j + 1]);
            }
            //left
            if (j - 1 >= 0)
            {
                sum += (0 * gs_vec[i][j - 1]);
            }
            //center
            sum += (0 * gs_vec[i][j]);
            //right
            if (j + 1 < orig_img.cols)
            {
                sum += (0 * gs_vec[i][j + 1]);
            }
            //bottom-left
            if (i + 1 < orig_img.rows && j - 1 >= 0)
            {
                sum += (-1 * gs_vec[i + 1][j - 1]);
            }
            //bottom-middle
            if (i + 1 < orig_img.rows)
            {
                sum += (-2 * gs_vec[i + 1][j]);
            }
            //bottom-right
            if (i + 1 < orig_img.rows && j + 1 < orig_img.cols)
            {
                sum += (-1 * gs_vec[i + 1][j + 1]);
            }
            //insert value
            gY[i][j] = sum;
        }

    }

    //COMPUTE MAGNITUDE VECTOR
    vector<vector<int>> magnitude(orig_img.rows, vector<int>(orig_img.cols, 0));
    for (int i = 0; i < orig_img.rows; i++)
    {
        for (int j = 0; j < orig_img.cols; j++)
        {
            magnitude[i][j] = (int)(sqrt(pow(gX[i][j], 2) + pow(gY[i][j], 2) * 1.0));
        }
    }

    //COMPUTE DIRECTION VECTOR
    vector<vector<double>> direction(orig_img.rows, vector<double>(orig_img.cols, 0));
    for (int i = 0; i < orig_img.rows; i++)
    {
        for (int j = 0; j < orig_img.cols; j++)
        {
            direction[i][j] = atan2(gY[i][j], gX[i][j]);
        }
    }

    //CLEAR OLD VECTORS TO FREE MEMORY
    vector<vector<int>>().swap(gX);
    vector<vector<int>>().swap(gY);

    //USE THRESHOLDING TO CREATE BINARY VECTOR
    /*vector<vector<int>> binary_vec(orig_img.rows, vector<int>(orig_img.cols, 0));
    int threshold = 170;
    for (int i = 0; i < orig_img.rows; i++)
    {
        for (int j = 0; j < orig_img.cols; j++)
        {
            if (magnitude[i][j] >= threshold)
            {
                binary_vec[i][j] = 1;
            }
            else
            {
                binary_vec[i][j] = 0;
            }
        }
    }*/

    //USE DIRECTION VECTOR TO DO HYSTHERESIS ON BINARY VECTOR
    vector<vector<int>> binary_vec(orig_img.rows, vector<int>(orig_img.cols, 0));
    int strong_threshold = 170;
    int weak_threshold = 85;
    for (int i = 0; i < orig_img.rows; i++)
    {
        for (int j = 0; j < orig_img.cols; j++)
        {
            if (magnitude[i][j] >= strong_threshold)//strong
            {
                binary_vec[i][j] = 1;
            }
            else if (magnitude[i][j] < weak_threshold)//suppressed
            {
                binary_vec[i][j] = 0;
            }
            else//weak edge
            {
                double direction_radians = direction[i][j];
                int center_magnitude = magnitude[i][j];
                if ((direction_radians >= -0.3926991 && direction_radians < 0.3926991) || (direction_radians < -2.7488936) || (direction_radians >= 2.7488936)) //left/right so perpendicular is up down
                {
                    if (i - 1 >= 0)//up
                    {
                        if (magnitude[i - 1][j] >= strong_threshold)
                        {
                            binary_vec[i][j] = 1;
                        }
                    }
                    if (i + 1 < orig_img.rows)//down
                    {
                        if (magnitude[i + 1][j] >= strong_threshold)
                        {
                            binary_vec[i][j] = 1;
                        }
                    }
                }
                else if ((direction_radians >= 0.3926991 && direction_radians < 1.178097) || (direction_radians >= -2.7488936 && direction_radians < -1.9634954))//right-up diagonal/left-down diagonal
                {
                    if (i - 1 >= 0 && j - 1 >= 0)//top-left
                    {
                        if (magnitude[i - 1][j - 1] >= strong_threshold)
                        {
                            binary_vec[i][j] = 1;
                        }
                    }
                    if (i + 1 < orig_img.rows && j + 1 < orig_img.cols)//bottom-right
                    {
                        if (magnitude[i + 1][j + 1] >= strong_threshold)
                        {
                            binary_vec[i][j] = 1;
                        }
                    }

                }
                else if ((direction_radians >= 1.178097 && direction_radians < 1.9634954) || (direction_radians >= -1.9634954 && direction_radians < -1.178097))//up/down
                {
                    if (j + 1 < orig_img.cols)//right
                    {
                        if (magnitude[i][j + 1] >= strong_threshold)
                        {
                            binary_vec[i][j] = 1;
                        }
                    }
                    if (j - 1 >= 0)//left
                    {
                        if (magnitude[i][j - 1] >= strong_threshold)
                        {
                            binary_vec[i][j] = 1;
                        }
                    }
                }
                else//left-up diagonal/right-down diagonal
                {
                    if (i - 1 >= 0 && j + 1 < orig_img.cols)//top-right
                    {
                        if (magnitude[i - 1][j + 1] >= strong_threshold)
                        {
                            binary_vec[i][j] = 1;
                        }
                    }
                    if (i + 1 < orig_img.rows && j - 1 >= 0)//bottom-left
                    {
                        if (magnitude[i + 1][j - 1] >= strong_threshold)
                        {
                            binary_vec[i][j] = 1;
                        }
                    }
                }
            }
        }
    }

    //USE DIRECTION VECTOR TO DO NON-MAXIMUM SUPPRESSION ON BINARY VECTOR
    for (int i = 0; i < orig_img.rows; i++)
    {
        for (int j = 0; j < orig_img.cols; j++)
        {
            double direction_radians = direction[i][j];
            int center_magnitude = magnitude[i][j];
            if ((direction_radians >= -0.3926991 && direction_radians < 0.3926991) || (direction_radians < -2.7488936) || (direction_radians >= 2.7488936)) //left/right
            {
                if (j + 1 < orig_img.cols)//right
                {
                    if (magnitude[i][j + 1] > center_magnitude)
                    {
                        binary_vec[i][j] = 0;
                    }
                }
                if (j - 1 >= 0)//left
                {
                    if (magnitude[i][j - 1] > center_magnitude)
                    {
                        binary_vec[i][j] = 0;
                    }
                }
            }
            else if ((direction_radians >= 0.3926991 && direction_radians < 1.178097) || (direction_radians >= -2.7488936 && direction_radians < -1.9634954))//right-up diagonal/left-down diagonal
            {
                if (i - 1 >= 0 && j + 1 < orig_img.cols)//top-right
                {
                    if (magnitude[i - 1][j + 1] > center_magnitude)
                    {
                        binary_vec[i][j] = 0;
                    }
                }
                if (i + 1 < orig_img.rows && j - 1 >= 0)//bottom-left
                {
                    if (magnitude[i + 1][j - 1] > center_magnitude)
                    {
                        binary_vec[i][j] = 0;
                    }
                }
            }
            else if ((direction_radians >= 1.178097 && direction_radians < 1.9634954) || (direction_radians >= -1.9634954 && direction_radians < -1.178097))//up/down
            {
                if (i - 1 >= 0)//up
                {
                    if (magnitude[i - 1][j] > center_magnitude)
                    {
                        binary_vec[i][j] = 0;
                    }
                }
                if (i + 1 < orig_img.rows)//down
                {
                    if (magnitude[i + 1][j] > center_magnitude)
                    {
                        binary_vec[i][j] = 0;
                    }
                }
            }
            else//left-up diagonal/right-down diagonal
            {
                if (i - 1 >= 0 && j - 1 >= 0)//top-left
                {
                    if (magnitude[i - 1][j - 1] > center_magnitude)
                    {
                        binary_vec[i][j] = 0;
                    }
                }
                if (i + 1 < orig_img.rows && j + 1 < orig_img.cols)//bottom-right
                {
                    if (magnitude[i + 1][j + 1] > center_magnitude)
                    {
                        binary_vec[i][j] = 0;
                    }
                }
            }
        }
    }

    
    //CONVERT VECTOR TO IMAGE
    Mat binary_img(orig_img.rows, orig_img.cols, CV_8U);
    for (int i = 0; i < orig_img.rows; i++)
    {
        for (int j = 0; j < orig_img.cols; j++)
        {
            if (binary_vec[i][j] == 1)
            {
                binary_img.at<uchar>(i, j) = 255;
            }
            else
            {
                binary_img.at<uchar>(i, j) = 0;
            }
        }
    }


    imshow("Binary image", binary_img);
    k = waitKey(0);
    cvtColor(binary_img, binary_img, COLOR_GRAY2BGR);
    imwrite("C:/Users/trish/OneDrive/Documents/JuniorYear/ComputerVision/Canny Edge Detection/Lab11/monarch_edges.ppm", binary_img);
    imwrite("C:/Users/trish/OneDrive/Documents/JuniorYear/ComputerVision/Canny Edge Detection/Lab11/monarch_edges.jpg", binary_img);
}