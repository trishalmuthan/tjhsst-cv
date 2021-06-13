#include <bits/stdc++.h> 
using namespace std; 

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

const int dimensions = 2; 

struct Node 
{ 
    double point[dimensions];
    Node *left, *right; 
}; 
  
//new node
struct Node* newNode(double values[]) 
{ 
    struct Node* store = new Node; 
    for (int i=0; i<dimensions; i++) 
    {
       store->point[i] = values[i]; 
    }
    store->left = store->right = NULL; 
    return store; 
} 
  
Node *insertHelp(Node *root, double newPoint[], int depth) 
{ 
    if (root == NULL) 
    {
       return newNode(newPoint); 
    }
    int cd = depth % dimensions; 
    if (newPoint[cd] < (root->point[cd])) 
    {
        root->left = insertHelp(root->left, newPoint, depth + 1); 
    }
    else
    {
        root->right = insertHelp(root->right, newPoint, depth + 1); 
    }
    return root; 
} 

Node* insert(Node *root, double newPoint[]) 
{ 
    return insertHelp(root, newPoint, 0); 
}

bool equals(double point1[], double point2[]) 
{ 
    for (int i = 0; i < dimensions; ++i) 
    {
        if (point1[i] != point2[i])
        { 
            return false;
        }
    }
    return true; 
} 

int searchHelp(Node* root, double point[], int depth) 
{ 
    if (root == NULL) 
    {
        return -1; 
    }
    if (equals(root->point, point)) 
    {
        int cd = depth % dimensions; 
        return cd; 
    }
    int cd = depth % dimensions; 
    if (point[cd] < root->point[cd]) 
    {
        return searchHelp(root->left, point, depth + 1);
    }
    return searchHelp(root->right, point, depth + 1); 
} 
  
int search(Node* root, double point[]) 
{ 
    return searchHelp(root, point, 0); 
} 

int main() 
{ 
    vector<vector<int>> picture(800, vector<int>(800, 0));
    srand(time(0));
    struct Node *root = NULL; 
    double points[10][dimensions];
    for(int r = 0; r < 10; r++)
    {   
        double value1;
        double value2;
        for (int j = 0; j < 2; j++)
        {
            points[r][0] = rand()/(double)RAND_MAX;
            points[r][1] = rand()/(double)RAND_MAX;
        }
    }
  
    int n = sizeof(points)/sizeof(points[0]); 
  
    for (int i=0; i<n; i++) 
       root = insert(root, points[i]); 

    for(int i = 0; i < n; i++)
    {
        int x_1 = (int)round(points[i][0] * 800);
        int y_1 = (int)round(points[i][1] * 800);
        createPoint(x_1, y_1, picture);
        double point1[] = {points[i][0], points[i][1]};
        int line_direct = (int)search(root, point1);
        if (line_direct == 0)
        {
            bresenham(x_1, y_1, x_1, 0, picture);
            bresenham(x_1, y_1, x_1, 800, picture);
        }
        if (line_direct == 1)
        {
            bresenham(x_1, y_1, 0, y_1, picture);
            bresenham(x_1, y_1, 800, y_1, picture);
        }
    }
    createppm(picture);

  
    return 0; 
} 