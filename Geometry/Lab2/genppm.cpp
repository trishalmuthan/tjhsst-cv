#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <cmath>
#include <vector>

using namespace std;

double distance(double x1, double y1, double x2, double y2);
double triangleArea(double x1, double x2, double x3, double y1, double y2, double y3);
void bresenham(int x1, int y1, int x2, int y2, vector<vector<int>> &picture);
double xIntersection(double a1, double b1, double c1, double a2, double b2, double c2);
double yIntersection(double a1, double b1, double c1, double a2, double b2, double c2);
void createPoint(int x1, int y1, vector<vector<int>> &picture);

int main()
{
    ofstream ofs("square.ppm");
    vector<vector<int>> picture(800, vector<int>(800, 0));
    srand(time(0));

    double x1, y1, x2, y2, x3, y3;
    double dist1, dist2, dist3;

    bool invalidLength = true;
    //Find random points and distances
    while(invalidLength)
    {
        x1 = (rand()/(double)RAND_MAX);
        y1 = (rand()/(double)RAND_MAX);
        x2 = (rand()/(double)RAND_MAX);
        y2 = (rand()/(double)RAND_MAX);
        x3 = (rand()/(double)RAND_MAX);
        y3 = (rand()/(double)RAND_MAX);
        dist1 = distance(x1, y1, x2, y2);
        dist2 = distance(x2, y2, x3, y3);
        dist3 = distance(x1, y1, x3, y3);
        if(dist1 > dist2 + dist3 || dist2 > dist1 + dist3 || dist3 > dist1 + dist2)
        {
            invalidLength = true;
        }
        else
        {
            invalidLength = false;
        }
    }
    
    bool invalidCoords = true;
    double x4, y4;
    while(invalidCoords)
    {
        x1 = (rand()/(double)RAND_MAX);
        y1 = (rand()/(double)RAND_MAX);
        x2 = (rand()/(double)RAND_MAX);
        y2 = (rand()/(double)RAND_MAX);
        x3 = (rand()/(double)RAND_MAX);
        y3 = (rand()/(double)RAND_MAX);
        x4 = (rand()/(double)RAND_MAX);
        y4 = (rand()/(double)RAND_MAX);
        //4 is center piece
        if(triangleArea(x1, y1, x2, y2, x4, y4) + triangleArea(x1, y1, x4, y4, x3, y3) + triangleArea(x4, y4, x2, y2, x3, y3) == triangleArea(x1, y1, x2, y2, x3, y3))
        {
            invalidCoords = true;
        }
        //3 is center piece
        if(triangleArea(x1, y1, x2, y2, x3, y3) + triangleArea(x1, y1, x4, y4, x3, y3) + triangleArea(x4, y4, x2, y2, x3, y3) == triangleArea(x1, y1, x2, y2, x4, y4))
        {
            invalidCoords = true;
        }
        //2 is center piece
        if(triangleArea(x1, y1, x2, y2, x3, y3) + triangleArea(x1, y1, x2, y2, x4, y4) + triangleArea(x2, y2, x3, y3, x4, y4) == triangleArea(x1, y1, x3, y3, x4, y4))
        {
            invalidCoords = true;
        }
        //1 is center piece
        if(triangleArea(x1, y1, x2, y2, x3, y3) + triangleArea(x1, y1, x4, y4, x3, y3) + triangleArea(x4, y4, x2, y2, x1, y1) == triangleArea(x4, y4, x2, y2, x3, y3))
        {
            invalidCoords = true;
        }
        else
        {
            invalidCoords = false;
        }
        
    }

    int x_1 = (int)round(x1 * 800);
    int x_2 = (int)round(x2 * 800);
    int x_3 = (int)round(x3 * 800);
    int x_4 = (int)round(x4 * 800);
    int y_1 = (int)round(y1 * 800);
    int y_2 = (int)round(y2 * 800);
    int y_3 = (int)round(y3 * 800);
    int y_4 = (int)round(y4 * 800);


    createPoint(x_1, y_1, picture);
    createPoint(x_2, y_2, picture);
    createPoint(x_3, y_3, picture);
    createPoint(x_4, y_4, picture);



    //Join AC
    double ACa = y3 - y1; 
    double ACb = x1 - x3; 
    double ACc = ACa*(x1)+ ACb*(y1); 
    
    //Create line BE which is perpendicular to AC and BE = AC
    double BEc = -1*ACb*x2 + ACa*y2;
    double BEa = -1*ACb;
    double BEb = ACa;


    double ACdistance = distance(x1, y1, x3, y3);
    //MIGHT NEED + OR -
    double Ex = x2 + sqrt( (ACdistance*ACdistance) / (1 + ((-BEa/BEb) * (-BEa/BEb)) ));
    double Ey = (BEc - (BEa*Ex))/BEb;

    int E_x = (int)round(Ex * 800);
    int E_y = (int)round(Ey * 800);

    //Line of Side D
    double sideDa = y4 - Ey;
    double sideDb = Ex - x4; 
    double sideDc =  sideDa*(Ex)+ sideDb*(Ey);

    //Line of Side A
    double sideAc = -1*sideDb*x1 + sideDa*y1;
    double sideAa = -1*sideDb;
    double sideAb = sideDa;

    //Line of Side B
    double sideBc = -1*sideAb*x2 + sideAa*y2;
    double sideBa = -1*sideAb;
    double sideBb = sideAa;

    //Line of Side C
    double sideCc = -1*sideBb*x3 + sideBa*y3;
    double sideCa = -1*sideBb;
    double sideCb = sideBa;

    double corner1x = xIntersection(sideAa, sideAb, sideAc, sideBa, sideBb, sideBc);
    double corner1y = yIntersection(sideAa, sideAb, sideAc, sideBa, sideBb, sideBc);
    
    double corner2x = xIntersection(sideBa, sideBb, sideBc, sideCa, sideCb, sideCc);
    double corner2y = yIntersection(sideBa, sideBb, sideBc, sideCa, sideCb, sideCc);
    
    double corner3x = xIntersection(sideCa, sideCb, sideCc, sideDa, sideDb, sideDc);
    double corner3y = yIntersection(sideCa, sideCb, sideCc, sideDa, sideDb, sideDc);

    double corner4x = xIntersection(sideDa, sideDb, sideDc, sideAa, sideAb, sideAc);
    double corner4y = yIntersection(sideDa, sideDb, sideDc, sideAa, sideAb, sideAc);

    int corner1_x = (int)round(corner1x * 800);
    int corner1_y = (int)round(corner1y * 800);
    int corner2_x = (int)round(corner2x * 800);
    int corner2_y = (int)round(corner2y * 800);
    int corner3_x = (int)round(corner3x * 800);
    int corner3_y = (int)round(corner3y * 800);
    int corner4_x = (int)round(corner4x * 800);
    int corner4_y = (int)round(corner4y * 800);

    bresenham(corner1_x, corner1_y, corner2_x, corner2_y, picture);
    bresenham(corner2_x, corner2_y, corner3_x, corner3_y, picture);
    bresenham(corner3_x, corner3_y, corner4_x, corner4_y, picture);
    bresenham(corner4_x, corner4_y, corner1_x, corner1_y, picture);


    ofs << "P3" << "  " << 800 << "  " << 800 << "  " << 1 << endl;
    for(int i = 0; i < 800; i++)
    {
        for(int j = 0; j < 800; j++)
        {
            ofs << picture[i][j] << " " << picture[i][j] << " " << picture[i][j] << " ";
        }
        ofs << endl;
    }

    return 0;
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

double xIntersection(double a1, double b1, double c1, double a2, double b2, double c2)
{
    double determinant = a1*b2 - a2*b1;
    double x = (b2*c1 - b1*c2)/determinant; 
    return x;
}

double yIntersection(double a1, double b1, double c1, double a2, double b2, double c2)
{
    double determinant = a1*b2 - a2*b1;
    double y = (a1*c2 - a2*c1)/determinant; 
    return y;
}


double distance(double x1, double y1, double x2, double y2) 
{ 
    // Calculating distance 
    return sqrt(pow(x2 - x1, 2) +  pow(y2 - y1, 2) * 1.0); 
} 

double triangleArea(double x1, double y1, double x2, double y2, double x3, double y3)
{
    double determinant = (x1*(y2-y3)) - (y1*(x2-x3)) + (1*((x2*y3) - (y2*x3)));
    return 0.5 * abs(determinant);
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