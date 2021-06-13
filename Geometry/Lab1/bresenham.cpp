#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <cmath>
#include <vector>

using namespace std;

void bresenham(int x1, int y1, int x2, int y2, vector<vector<int>> &picture)
{
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

int main()
{
    vector<vector<int>> picture(800, vector<int>(800, 0));

    int x1 = 250;
    int y1 = 250;

    int x2 = 750;
    int y2 = 750;
    

    bresenham(x1, y1, x2, y2, picture);


    ofstream ofs("hello.ppm");
    for(int i = 0; i < 800; i++)
    {
        for(int j = 0; j < 800; j++)
        {
            ofs << picture[i][j];
        }
        ofs << endl;
    }


}