#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <cmath>
#include <vector>
#include <algorithm>

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
};


double distance(double x1, double y1, double x2, double y2);
vector<Point> brute_force(vector<Point> &points);
vector<Point> divide_and_conquer(vector<Point> &pointsX, vector<Point> &pointsY);
void bresenham(int x1, int y1, int x2, int y2, vector<vector<int>> &picture);
void createPoint(int x1, int y1, vector<vector<int>> &picture);
void createppm(vector<vector<int>> &picture);


int main()
{
    ofstream ofs("points.ppm");
    vector<vector<int>> picture(800, vector<int>(800, 0));
    vector<Point> pointsX;
    vector<Point> pointsY;
    srand(time(0));
    for(int i = 0; i < 40; i++)
    {
        double x1 = rand()/(double)RAND_MAX;
        double y1 = rand()/(double)RAND_MAX;
        pointsX.push_back(Point(x1, y1));
        pointsY.push_back(Point(x1, y1));
        int x_1 = (int)round(x1 * 800);
        int y_1 = (int)round(y1 * 800);
        createPoint(x_1, y_1, picture);
    }

    sort(pointsY.begin(), pointsY.end(), [](const Point& p1, const Point& p2) {
      return p1.y < p2.y;
    });

   sort(pointsX.begin(), pointsX.end(), [](const Point& p1, const Point& p2) {
      return p1.x < p2.x;
    });

    vector<Point> dc = divide_and_conquer(pointsX, pointsY);

    int dcx1 = (int)round(dc[0].x * 800);
    int dcy1 = (int)round(dc[0].y * 800);
    int dcx2 = (int)round(dc[1].x * 800);
    int dcy2 = (int)round(dc[1].y * 800);

    bresenham(dcx1, dcy1, dcx2, dcy2, picture);

    createppm(picture);
}

void createppm(vector<vector<int>> &picture)
{
    ofstream ofs("points.ppm");
    ofs << "P3" << "  " << 800 << "  " << 800 << "  " << 255 << endl;
    for(int i = 0; i < 800; i++)
    {
        for(int j = 0; j < 800; j++)
        {
            if(picture[i][j] == 2)
            {
                ofs << 255 << " " << 0 << " " << 0 << " ";
            }
            else if(picture[i][j] == 1)
            {
                ofs << 255 << " " << 255 << " " << 255 << " ";
            }
            else
            {
                ofs << 0 << " " << 0 << " " << 0 << " ";
            }      
        }
        ofs << endl;
    }
}

//Divide and conquer algorithm p2
vector<Point> divide_and_conquer(vector<Point> &pointsX, vector<Point> &pointsY)
{
    //Base case if there are less than or equal to 3 points
    if(pointsX.size() <= 3)
    {  
        vector<Point> closestPair = brute_force(pointsX);
        return closestPair;
    }

    int mid = pointsX.size()/2;
    Point midPoint = pointsX[mid];

    //Divide into two halves, left and right
    vector<Point> leftPointsX(pointsX.begin(), pointsX.begin() + mid);
	vector<Point> rightPointsX(pointsX.begin() + mid, pointsX.end());

    vector<Point> leftPointsY;
    vector<Point> rightPointsY;
    for(int i = 0; i < pointsY.size(); i++)
    {
        if(pointsY[i].x < midPoint.x)
        {
            leftPointsY.push_back(pointsY[i]);
        }
        else
        {
            rightPointsY.push_back(pointsY[i]);

        }
    }
    //Recursively call to get closest pair on left and right half
    vector<Point> leftClosestPair = divide_and_conquer(leftPointsX, leftPointsY);
    vector<Point> rightClosestPair = divide_and_conquer(rightPointsX, rightPointsY);

    double leftDistance = distance(leftClosestPair[0].x, leftClosestPair[0].y, leftClosestPair[1].x, leftClosestPair[1].y);
    double rightDistance = distance(rightClosestPair[0].x, rightClosestPair[0].y, rightClosestPair[1].x, rightClosestPair[1].y);
    double delta = min(leftDistance, rightDistance);

    vector<Point> splitVector; //sorted by Y

    for (int k = 0; k < pointsY.size(); k++)
    {
        if(abs(pointsY[k].x - midPoint.x) < delta)
        {
            splitVector.push_back(pointsY[k]);
        }
    }

    if(splitVector.size() > 1) 
    {
        Point currentClosest1 = splitVector[0];
        Point currentClosest2 = splitVector[1];
        double minDistance = distance(currentClosest1.x, currentClosest1.y, currentClosest2.x, currentClosest2.y);
        for(int c = 0; c < splitVector.size(); c++)
        {
            for(int s = 1; s < 8; s++)
            {
                if(c+s < splitVector.size())
                {
                    double currDistance = distance(splitVector[c].x, splitVector[c].y, splitVector[c+s].x, splitVector[c+s].y);
                    if(currDistance < minDistance)
                    {
                        minDistance = currDistance;
                        currentClosest1 = splitVector[c];
                        currentClosest2 = splitVector[c+s];
                    }
                }
            }
        }
        double lowest = min(delta, minDistance);
        if(lowest == leftDistance)
        {
            return leftClosestPair;
        }
        else if(lowest == rightDistance)
        {
            return rightClosestPair;
        }
        else
        {
            vector<Point> stripPair = {currentClosest1, currentClosest2};
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
                picture[i][j] = 2;
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
                picture[i][j] = 2;
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
                picture[j][i] = 2;
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
                picture[j][i] = 2;
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