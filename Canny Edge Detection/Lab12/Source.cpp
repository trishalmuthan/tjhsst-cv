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

double find_intercept(double slope, int x, int y);
bool plug_in_x_max(double slope, double y_int, int rows, int cols);
bool plug_in_x_min(double slope, double y_int, int rows, int cols);
bool plug_in_y_max(double slope, double y_int, int rows, int cols);
bool plug_in_y_min(double slope, double y_int, int rows, int cols);
void bresenham(int x1, int y1, int x2, int y2, vector<vector<int>>& picture);
double distance(int x1, int y1, int x2, int y2);
double calc_hypotenuse(int rows, int columns);


int main()
{
    //READ IN IMAGE
    Mat orig_img = imread("C:/users/trish/OneDrive/Pictures/houghcoinseasy2.jpg", IMREAD_COLOR);
    //imshow("Original image", orig_img);
    //int k = waitKey(0);

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
    //vector<vector<int>>().swap(gX);
    //vector<vector<int>>().swap(gY);

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

    /*//CONVERT VECTOR TO IMAGE
    Mat binary_img(orig_img.rows, orig_img.cols, CV_8U);
    for (int i = 0; i < orig_img.rows; i++)
    {
        for (int j = 0; j < orig_img.cols; j++)
        {
            if (binary_vec[i][j] == 1)
            {
                binary_img.at<uchar>(i, j) = 255;
            }
            /*if (binary_vec[i][j] == 2)
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
    cvtColor(binary_img, binary_img, COLOR_GRAY2BGR);*/
    
    //FIRST ROUND OF VOTING TO FIND CENTERS + STORING EDGE PIXELS
    vector<pair<int, int>> edges;
    vector<vector<int>> first_ballot(orig_img.rows, vector<int>(orig_img.cols, 0));
    for (int i = 0; i < orig_img.rows; i++)
    {
        for (int j = 0; j < orig_img.cols; j++)
        {
            if (binary_vec[i][j] == 1)
            {
                pair<int, int> edge(i, j);
                edges.push_back(edge);
                double slope = -1/float(tan(direction[i][j]));
                //cout << "slope " << slope << endl;
                //cout << "point " << i << " " << j << endl;
                double y_int = find_intercept(slope, i, j);
                //cout << y_int << endl;
                if (plug_in_x_max(slope, y_int, orig_img.rows, orig_img.cols))
                {
                    //cout << "xmax" << endl;
                    int newX = orig_img.rows - 1;
                    int newY = int(slope * (orig_img.rows - 1) + y_int);
                    //cout << "endpoint " << newX << " " << newY << endl;
                    bresenham(i, j, newX, newY, first_ballot);
                }

                if (plug_in_x_min(slope, y_int, orig_img.rows, orig_img.cols))
                {
                    //cout << "xmin" << endl;
                    int newX = 0;
                    int newY = int(slope * (0) + y_int);
                    //cout << "endpoint " << newX << " " << newY << endl;

                    bresenham(i, j, newX, newY, first_ballot);
                }

                if (plug_in_y_max(slope, y_int, orig_img.rows, orig_img.cols))
                {
                    //cout << "ymax" << endl;
                    int newY = orig_img.cols - 1;
                    int newX = int(((orig_img.cols - 1) - y_int) / double(slope));
                    //cout << "endpoint " << newX << " " << newY << endl;

                    bresenham(i, j, newX, newY, first_ballot);
                }

                if (plug_in_y_min(slope, y_int, orig_img.rows, orig_img.cols))
                {
                    //cout << "ymin" << endl;
                    int newY = 0;
                    int newX = int(((0) - y_int) / double(slope));
                    //cout << "endpoint " << newX << " " << newY << endl;

                    bresenham(i, j, newX, newY, first_ballot);
                }
                //cout << "slope " << slope << endl;
                //cout << "point " << i << " " << j << endl;
                //cout << y_int << endl;
                //FIND NEW X AND NEW Y
                //bresenham(i, j, newX, newY, first_ballot);
            }
        }
    }
    //THRESHOLD TO FIND POINTS WITH MOST VOTES AND ADD TO CENTERS VECTOR
    /*vector<pair<int, int>> centers;
    for (int i = 0; i < orig_img.rows; i++)
    {
        for (int j = 0; j < orig_img.cols; j++)
        {
            if (first_ballot[i][j] >= 60)
            {

                bool too_close = false;
                for (int z = 0; z < centers.size(); z++)
                {
                    int x1 = centers[z].first;
                    int y1 = centers[z].second;
                    if (distance(x1, y1, i, j) < 40)
                    {
                        too_close = true;
                    }
                }
                //cout << first_ballot[i][j] << " ";
                if (!too_close)
                {
                    pair<int, int> center(i, j);
                    centers.push_back(center);
                }
            }
        }
        //cout << endl;
    }*/

    vector<pair<int, int>> centers;
    for (int i = 0; i < orig_img.rows; i++)
    {
        for (int j = 0; j < orig_img.cols; j++)
        {
            if (first_ballot[i][j] >= 60 && !(i < 10) && !(i > orig_img.rows-10) && !(j < 10) && !(j > orig_img.cols - 10))
            {
                 //cout << i << endl;
                 pair<int, int> center(i, j);
                 centers.push_back(center);
            }
        }
        //cout << endl;
    }


    /*for (int i = 0; i < centers.size(); i++) 
    {
        int x1 = centers[i].first;
        int y1 = centers[i].second;
        binary_vec[x1][y1] = 2;
    }*/
    //cout << "second vallot started" << endl;
    //cout << centers.size() << endl;
    for (int i = 0; i < centers.size(); i++)
    {
        //cout << i << endl;
        vector<vector<pair<int, int>>> second_ballot(int(round(calc_hypotenuse(orig_img.rows, orig_img.cols))));
        for (int e = 0; e < edges.size(); e++)
        {
            //calc distance between edge and center and add the edge to the vector at the corresponding distance index in the second_ballot
            pair<int, int> addEdge(edges[e].first, edges[e].second);
            second_ballot[int(round(distance(centers[i].first, centers[i].second, edges[e].first, edges[e].second)))].push_back(addEdge);
        }
        int cur_max_size = 0;
        int cur_index_max = 0;
        for (int k = 2; k < 280; k++)
        {
            if (second_ballot[k].size()+ second_ballot[k-1].size()+ second_ballot[k+1].size() + second_ballot[k - 2].size() + second_ballot[k + 2].size() > cur_max_size)
            {
                cur_max_size = second_ballot[k].size() + second_ballot[k - 1].size() + second_ballot[k + 1].size() + second_ballot[k - 2].size() + second_ballot[k + 2].size();
                cur_index_max = k;
            }
        }
        //if (cur_index_max > 280)
        //{
         //   cout << "what" << endl;
       // }
        //cout << cur_index_max << endl;
        //cout << second_ballot[cur_index_max].size() << endl;
        for (int r = 0; r < second_ballot[cur_index_max].size(); r++)
        {
            int x_val = second_ballot[cur_index_max][r].first;
            int y_val = second_ballot[cur_index_max][r].second;

                binary_vec[x_val][y_val] = 3;
        }
    }

    
    //CONVERT VECTOR TO IMAGE
    Mat binary_img(orig_img.rows, orig_img.cols, CV_8U);
    for (int i = 0; i < orig_img.rows; i++)
    {
        for (int j = 0; j < orig_img.cols; j++)
        {
            /*if (i == 40)
            {
                //cout << "point " << i << " " << j << endl;
                binary_img.at<uchar>(i, j) = 255;
            }*/
            /*if (binary_vec[i][j] == 1)
            {
                //cout << "point " << i << " " << j << endl;
                binary_img.at<uchar>(i, j) = 0;
            }
            else if (binary_vec[i][j] == 2)
            {
                binary_img.at<uchar>(i, j) = 0;
            }*/
            if (binary_vec[i][j] == 3)
            {
                orig_img.at<Vec3b>(i, j)[0] = 255;
                orig_img.at<Vec3b>(i, j)[1] = 0;
                orig_img.at<Vec3b>(i, j)[2] = 0;
            }
            /*else
            {
                orig_img.at<Vec3b>(i, j)[0] = 0;
                orig_img.at<Vec3b>(i, j)[1] = 0;
                orig_img.at<Vec3b>(i, j)[2] = 0;
            }*/
        }
    }
    //cout << binary_img << endl;
    
    
    cout << 5.56 << endl;

    //imshow("Binary image", orig_img);
    //int k = waitKey(0);
    //cvtColor(binary_img, binary_img, COLOR_GRAY2BGR);
    //imwrite("C:/Users/trish/OneDrive/Documents/JuniorYear/ComputerVision/Canny Edge Detection/Lab12/coinedges.ppm", orig_img);
    imwrite("C:/Users/trish/OneDrive/Documents/JuniorYear/ComputerVision/Canny Edge Detection/Lab12/coinedges.jpg", orig_img);
}

// Calculating distance 
double distance(int x1, int y1, int x2, int y2)
{
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2) * 1.0);
}

double calc_hypotenuse(int rows, int columns)
{
    return sqrt(pow(rows, 2) + pow(columns, 2) * 1.0);
}


bool plug_in_x_max(double slope, double y_int, int rows, int cols)
{
    int y_val = int(slope * (rows - 1) + y_int);
    if (y_val >= 0 && y_val < cols)
    {
        return true;
    }
    return false;
}

bool plug_in_x_min(double slope, double y_int, int rows, int cols)
{
    int y_val = int(slope * (0) + y_int);
    if (y_val >= 0 && y_val < cols)
    {
        return true;
    }
    return false;
}

bool plug_in_y_max(double slope, double y_int, int rows, int cols)
{
    int x_val = int(((cols - 1) - y_int) / slope);
    if (x_val >= 0 && x_val < rows)
    {
        return true;
    }
    return false;
}

bool plug_in_y_min(double slope, double y_int, int rows, int cols)
{
    int x_val = int((0 - y_int) / slope);
    if (x_val >= 0 && x_val < rows)
    {
        return true;
    }
    return false;
}

double find_intercept(double slope, int x, int y)
{
    return (y - slope * x);
}

void bresenham(int x1, int y1, int x2, int y2, vector<vector<int>>& picture)
{
    /*if (x1 >= 800)
    {
        x1 = 799;
    }
    if (y1 >= 800)
    {
        y1 = 799;
    }
    if (x2 >= 800)
    {
        x2 = 799;
    }
    if (y2 >= 800)
    {
        y2 = 799;
    }*/
    int deltax = x2 - x1;
    int deltay = y2 - y1;
    if (abs(deltax) >= abs(deltay))
    {
        int e = abs(deltay) - abs(deltax);
        if (deltax > 0)
        {
            int j = y1;
            for (int i = x1; i < x2 - 1; i++)
            {
                picture[i][j] += 1;
                if (e >= 0)
                {
                    if (y1 < y2)
                    {
                        j += 1;
                    }
                    else
                    {
                        j -= 1;
                    }
                    e -= abs(deltax);
                }
                e += abs(deltay);
            }
        }
        else
        {
            int j = y2;
            for (int i = x2; i < x1 - 1; i++)
            {
                picture[i][j] += 1;
                if (e >= 0)
                {
                    if (y1 < y2)
                    {
                        j -= 1;
                    }
                    else
                    {
                        j += 1;
                    }
                    e -= abs(deltax);
                }
                e += abs(deltay);
            }
        }
    }
    else
    {
        int e = abs(deltax) - abs(deltay);
        if (deltay > 0)
        {
            int j = x1;
            for (int i = y1; i < y2 - 1; i++)
            {
                picture[j][i] += 1;
                if (e >= 0)
                {
                    if (x1 > x2)
                    {
                        j -= 1;
                    }
                    else
                    {
                        j += 1;
                    }
                    e -= abs(deltay);
                }
                e += abs(deltax);
            }
        }
        else
        {
            int j = x2;
            for (int i = y2; i < y1 - 1; i++)
            {
                picture[j][i] += 1;
                if (e >= 0)
                {
                    if (x1 > x2)
                    {
                        j += 1;
                    }
                    else
                    {
                        j -= 1;
                    }
                    e -= abs(deltay);
                }
                e += abs(deltax);
            }
        }
    }
}