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

//Hash function
struct pair_hash
{
	template <class T1, class T2>
	std::size_t operator() (const std::pair<T1, T2> &pair) const
	{
		return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
	}
};

vector<Point> brute_force(vector<Point> &points);
vector<Point> divide_and_conquer(vector<Point> &pointsX, vector<Point> &pointsY);
vector<Point> randomized_closest_pair(vector<Point> &points);
void bresenham(int x1, int y1, int x2, int y2, vector<vector<int>> &picture);
void createPoint(int x1, int y1, vector<vector<int>> &picture);
void KNrandomize(vector<Point> &points);
void createppm(vector<vector<int>> &picture);
double distance(double x1, double y1, double x2, double y2);


int main()
{
    vector<vector<int>> picture(800, vector<int>(800, 0));
    vector<Point> points;
    srand(time(0));
    for(int i = 0; i < 35; i++)
    {
        double x1 = rand()/(double)RAND_MAX;
        double y1 = rand()/(double)RAND_MAX;
        points.push_back(Point(x1, y1));
        int x_1 = (int)round(x1 * 800);
        int y_1 = (int)round(y1 * 800);
        createPoint(x_1, y_1, picture);
    }

    vector<Point> rp = randomized_closest_pair(points);
    int rpx1 = (int)round(rp[0].x * 800);
    int rpy1 = (int)round(rp[0].y * 800);
    int rpx2 = (int)round(rp[1].x * 800);
    int rpy2 = (int)round(rp[1].y * 800);

    bresenham(rpx1, rpy1, rpx2, rpy2, picture);
    createppm(picture);
}

//Randomized Closest Pair Algorithm
vector<Point> randomized_closest_pair(vector<Point> &points)
{
    KNrandomize(points);
    Point closest_pair1 = points[0];
    Point closest_pair2 = points[1];
    double min_delta = distance(points[0].x, points[0].y, points[1].x, points[1].y);
    double d = min_delta / 2;
    int max_row = (int)ceil(1/d)-1;
    int max_col = (int)ceil(1/d)-1;
    unordered_map<pair<int, int>, vector<Point>, pair_hash> subsquares;
    for(int i = 0; i < points.size(); i++)
    {
        Point current = points[i];
        int subsquare_row = ((int)(floor(current.x/d)));
        int subsquare_col = ((int)(floor(current.y/d)));
        int start_row;
        int end_row;
        int start_col;
        int end_col;

        if(subsquare_row - 2 < 0)
        {
            start_row = 0;
        }
        else
        {
            start_row = subsquare_row - 2;
        }

        if(subsquare_row + 2 > max_row)
        {
            end_row = max_row;
        }
        else
        {
            end_row = subsquare_row + 2;
        }

        if(subsquare_col - 2 < 0)
        {
            start_col = 0;
        }
        else
        {
            start_col = subsquare_col - 2;
        }

        if(subsquare_col + 2 > max_col)
        {
            end_col = max_col;
        }
        else
        {
            end_col = subsquare_col + 2;
        }

        bool newClosest = false;
        for(int r = start_row; r <= end_row; r++)
        {
            for(int c = start_col; c <= end_col; c++)
            {
                pair<int, int> subsq(r, c);
                if(subsquares.find(subsq) != subsquares.end())
                {
                    vector<Point> subsquare_points = subsquares[subsq];
                    for(int p = 0; p < subsquare_points.size(); p++)
                    {
                        if(distance(subsquare_points[p].x, subsquare_points[p].y, current.x, current.y) < min_delta)
                        {
                            closest_pair1 = current;
                            closest_pair2 = subsquare_points[p];
                            min_delta = distance(subsquare_points[p].x, subsquare_points[p].y, current.x, current.y);
                            newClosest = true;
                        }
                    }
                }
            }
        }
        if(newClosest)
        {
            d = min_delta/2;
            subsquares.clear();
            for(int o = 0; o <= i; o++)
            {
                int new_subsquare_row = (int)floor(points[o].x/d);
                int new_subsquare_col = (int)floor(points[o].y/d);
                pair<int, int> ins(new_subsquare_row, new_subsquare_col);
                if(subsquares.find(ins) != subsquares.end())
                {
                    subsquares[ins].push_back(points[o]);
                }
                else
                {
                    subsquares[ins] = vector<Point>();
                    subsquares[ins].push_back(points[o]);
                }
            }
            max_row = (int)ceil(1/d)-1;
            max_col = (int)ceil(1/d)-1;
        }
        else
        {
            pair<int, int> ins(subsquare_row, subsquare_col);
            if(subsquares.find(ins) != subsquares.end())
            {
                subsquares[ins].push_back(current);
            }
            else
            {
                subsquares[ins] = vector<Point>();
                subsquares[ins].push_back(current);
            }
        }
    }

    vector<Point> finalPair = {closest_pair1, closest_pair2};
    return finalPair;
}

//Knuth Shuffle Randomization
void KNrandomize(vector<Point> &points)
{
    for (int i = points.size()-1; i > 0; i--)
    {
        int j = rand() % (i+1);
        Point temp = points[i];
        points[i] = points[j];
        points[j] = temp;
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