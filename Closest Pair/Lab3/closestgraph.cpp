#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <cmath>
#include <vector>
#include <algorithm>
#include <chrono>


using namespace std;

//Point class
class Point
{
    public:
        //xcoordinate and ycoordinate
        double x;
        double y;
        Point(double xcoord, double ycoord)
        {
            x = xcoord;
            y = ycoord;
        }
        //Comparator
        bool operator < (const Point& p) const 
        {
            return (x < p.x);
        }
};

double distance(double x1, double y1, double x2, double y2);
vector<Point> brute_force(vector<Point> &points);
vector<Point> divide_and_conquer(vector<Point> &points);
void bresenham(int x1, int y1, int x2, int y2, vector<vector<int>> &picture);
void createPoint(int x1, int y1, vector<vector<int>> &picture);



int main()
{
    /*ofstream ofs("points.ppm");
    vector<vector<int>> picture(800, vector<int>(800, 0));*/
    vector<Point> points;
    srand(time(0));
    for(int i = 0; i < 65536; i++)
    {
        double x1 = rand()/(double)RAND_MAX;
        double y1 = rand()/(double)RAND_MAX;
        points.push_back(Point(x1, y1));
        int x_1 = (int)round(x1 * 800);
        int y_1 = (int)round(y1 * 800);
        //createPoint(x_1, y_1, picture);
    }

    
    auto start = chrono::high_resolution_clock::now();
    auto stop = chrono::high_resolution_clock::now(); 
    auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
    int count = 0;
    for(int j = 0; j < 3; j++)
    {
        start = chrono::high_resolution_clock::now();
        vector<Point> dc = brute_force(points);
        stop = chrono::high_resolution_clock::now(); 
        duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
        count += duration.count();
    }
    cout << count / 3.0 << endl;

    /*start = chrono::high_resolution_clock::now();
    vector<Point> br = brute_force(points);
    cout << distance(br[0].x, br[0].y, br[1].x, br[1].y) << endl;
    stop = chrono::high_resolution_clock::now(); 
    duration = chrono::duration_cast<chrono::milliseconds>(stop - start);

    cout << duration.count() << endl;*/

    /*int dcx1 = (int)round(dc[0].x * 800);
    int dcy1 = (int)round(dc[0].y * 800);
    int dcx2 = (int)round(dc[1].x * 800);
    int dcy2 = (int)round(dc[1].y * 800);
    cout << dcx1 << " " << dcy1 << " " << dcx2 << " " << dcy2 << endl;*/


    //bresenham(dcx1, dcy1, dcx2, dcy2, picture);

    /*ofs << "P3" << "  " << 800 << "  " << 800 << "  " << 1 << endl;
    for(int i = 0; i < 800; i++)
    {
        for(int j = 0; j < 800; j++)
        {
            ofs << picture[i][j] << " " << picture[i][j] << " " << picture[i][j] << " ";
        }
        ofs << endl;
    }*/
}

//Divide and conquer algorithm p1
vector<Point> divide_and_conquer(vector<Point> &points)
{
    //Base case if there are less than or equal to 3 points
    if(points.size() <= 3)
    {  
        vector<Point> closestPair = brute_force(points);
        return closestPair;
    }

    //Sort the points by x coord
    sort(points.begin(), points.end());

    //Divide into two halves, left and right
    vector<Point> leftPoints(points.begin(), points.begin() + points.size() / 2);
	vector<Point> rightPoints(points.begin() + points.size() / 2, points.end());

    //Recursively call to get closest pair on left and right half
    vector<Point> leftClosestPair = divide_and_conquer(leftPoints);
    vector<Point> rightClosestPair = divide_and_conquer(rightPoints);

    double leftDistance = distance(leftClosestPair[0].x, leftClosestPair[0].y, leftClosestPair[1].x, leftClosestPair[1].y);
    double rightDistance = distance(rightClosestPair[0].x, rightClosestPair[0].y, rightClosestPair[1].x, rightClosestPair[1].y);
    double delta = min(leftDistance, rightDistance);

    double midValue = points[points.size()/2].x;
    double leftdelta = midValue - delta;
    double rightdelta = midValue + delta;

    vector<Point> splitVector;

    for (int k = 0; k < points.size(); k++)
    {
        if(points[k].x > leftdelta && points[k].x < rightdelta)
        {
            splitVector.push_back(points[k]);
        }
    }

    if(splitVector.size() > 2)
    {
        vector<Point> stripPair = brute_force(splitVector);
        double stripDistance = distance(stripPair[0].x, stripPair[0].y, stripPair[1].x, stripPair[1].y);
        double minDistance = min(delta, stripDistance);
        if(minDistance == leftDistance)
        {
            return leftClosestPair;
        }
        else if(minDistance == rightDistance)
        {
            return rightClosestPair;
        }
        else
        {
            return stripPair;
        }
    }
    else
    {
        if(delta == leftDistance)
        {
            return leftClosestPair;
        }
        else
        {
            return rightClosestPair;
        }
    }


}

//Brute force algorithm
vector<Point> brute_force(vector<Point> &points)
{
    //Store first 2 points and their distance
    Point currentClosest1 = points[0];
    Point currentClosest2 = points[1];
    double currentDistance = distance(currentClosest1.x, currentClosest1.y, currentClosest2.x, currentClosest2.y);
    double tempDistance;
    //Go through every pair of points
    for(int i = 0; i < points.size(); i++)
    {
        for(int j = 0; j < points.size(); j++)
        {
            //If not equal
            if (i != j)
            {
                //If distance is less than current, switch up the points
                tempDistance = distance(points[i].x, points[i].y, points[j].x, points[j].y);
                if (tempDistance < currentDistance)
                {
                    currentClosest1 = points[i];
                    currentClosest2 = points[j];
                    currentDistance = tempDistance;
                }
            }
        }
    }

    //Print
    /*cout << currentClosest1.x << " " << currentClosest1.y << endl;
    cout << currentClosest2.x << " " << currentClosest2.y << endl;
    cout << currentDistance;*/

    //Return vector of points
    vector<Point> closest = {currentClosest1, currentClosest2};
    return closest;
}


// Calculating distance 
double distance(double x1, double y1, double x2, double y2) 
{ 
    return sqrt(pow(x2 - x1, 2) +  pow(y2 - y1, 2) * 1.0); 
} 

void bresenham(int x1, int y1, int x2, int y2, vector<vector<int>> &picture)
{
    if (x1 >= 800)
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
    }
    int deltax = x2-x1;
    int deltay = y2-y1;
    if(abs(deltax) >= abs(deltay))
    {   
        int e = abs(deltay)-abs(deltax);
        if(deltax > 0)
        {
            int j = y1;
            for(int i = x1; i < x2-1; i++)
            {
                picture[i][j] = 1;
                if(e >= 0)
                {
                    if(y1 < y2)
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
            for(int i = x2; i < x1-1; i++)
            {
                picture[i][j] = 1;
                if(e >= 0)
                {
                    if(y1 < y2)
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
        int e = abs(deltax)-abs(deltay);
        if(deltay > 0)
        {
            int j = x1;
            for(int i = y1; i < y2-1; i++)
            {
                picture[j][i] = 1;
                if(e >= 0)
                {
                    if(x1 > x2)
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
            for(int i = y2; i < y1-1; i++)
            {
                picture[j][i] = 1;
                if(e >= 0)
                {
                    if(x1 > x2)
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

void createPoint(int x1, int y1, vector<vector<int>> &picture)
{
    picture[x1][y1] = 1;
    if (x1 > 0)
    {
        picture[x1-1][y1] = 1;
    }
    if (x1 < 799)
    {
        picture[x1+1][y1] = 1;
    }
    if (y1 > 0)
    {
        picture[x1][y1-1] = 1;
    }
    if (y1 < 799)
    {
        picture[x1][y1+1] = 1;
    }
    if (x1 > 0 && y1 > 0)
    {
        picture[x1-1][y1-1] = 1;
    }
    if (x1 < 799 && y1 > 0)
    {
        picture[x1+1][y1-1] = 1;
    }
    if (x1 > 0 && y1 < 799)
    {
        picture[x1-1][y1+1] = 1;
    }
    if (x1 > 0 && y1 < 799)
    {
        picture[x1+1][y1+1] = 1;
    }    
}