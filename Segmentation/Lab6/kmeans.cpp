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
#include <cfloat>

using namespace std;

//Point class
class Point
{
    public:
        //xcoordinate and ycoordinate
        int cluster;
        vector<double> values;
        Point(vector<double> inputValues, int inputCluster)
        {
            cluster = inputCluster;
            values = inputValues;
        }
};

double distance(Point p1, Point p2);
pair<vector<Point>, vector<Point>> kmeans_clustering_with_epochs(vector<Point> points, int dimensions, int num_clusters, int epochs);
pair<vector<Point>, vector<Point>>kmeans_clustering_with_convergence(vector<Point> points, int dimensions, int num_clusters, double convergence_limit);
void createPoint(int x1, int y1, int cluster, vector<vector<int>> &picture);
void createppm(vector<vector<int>> &picture);
double calc_variation(vector<Point> centroids, vector<Point> points, int dimensions, int num_clusters);


int main()
{
    vector<vector<int>> picture(800, vector<int>(800, 0));
    srand(time(0));
    //Generate points in 2 dimensions
    vector<Point> points;
    for(int r = 0; r < 100; r++)
    {
        vector<double> coordinates;
        for (int j = 0; j < 2; j++)
        {
            double value = rand()/(double)RAND_MAX;
            coordinates.push_back(value);
        }
        points.push_back(Point(coordinates, -1));
    }
    
    pair<vector<Point>, vector<Point>> points_centroids = kmeans_clustering_with_epochs(points, 2, 6, 40);
    points = points_centroids.first;
    vector<Point> centroids = points_centroids.second;
    //Print out the centroids
    for(int i = 0; i < points.size(); i++)
    {
        vector<double> coords = points[i].values;
        int cluster = points[i].cluster;
        int x_1 = (int)round(coords[0] * 800);
        int y_1 = (int)round(coords[1] * 800);
        createPoint(x_1, y_1, cluster, picture);
    }
    createppm(picture);
}

pair<vector<Point>, vector<Point>> kmeans_clustering_with_epochs(vector<Point> points, int dimensions, int num_clusters, int epochs)
{
    //Initialize random set of centroids/means
    vector<Point> centroids;
    for (int i = 0; i < num_clusters; i++)
    {
        vector<double> coordinates;
        for (int j = 0; j < dimensions; j++)
        {
            double value = rand()/(double)RAND_MAX;
            coordinates.push_back(value);
        }
        centroids.push_back(Point(coordinates, i));
    }
    //Classify every point into a cluster
    for (int i = 0; i < points.size(); i++)
    {
        double min_distance = DBL_MAX;
        double min_cluster;
        for (int j = 0; j < centroids.size(); j++)
        {
            double cur_distance = distance(points[i], centroids[j]);
            if(cur_distance < min_distance)
            {
                min_distance = cur_distance;
                min_cluster = j;
            }
        }
        points[i].cluster = min_cluster;
    }

    for(int iteration = 0; iteration < epochs; iteration++)
    {
        //Recompute centroids
        vector<vector<double>> sums;//Stores sum of all individual coordinates for every cluster
        vector<int> counts;//Store number of points in each cluster
        for(int cluster = 0; cluster < num_clusters; cluster++)//For every cluster
        {
            //Create a vector to store the sum for that cluster and start all the values at 0
            vector<double> store_values;
            for(int z = 0; z < dimensions; z++)
            {
                store_values.push_back(0.0);
            }
            sums.push_back(store_values);
            //Start the count for that cluster at 0
            counts.push_back(0);
        }
        for (int p = 0; p < points.size(); p++)//For every point
        {
            int current_cluster = points[p].cluster;//Get its cluster
            counts[current_cluster] += 1; //Increase the count at that cluster by 1
            vector<double> current_values = sums[current_cluster]; //Get the current sums of every coord. at that cluster
            for(int s = 0; s < points[p].values.size(); s++)//For every coord in the current point
            {
                current_values[s] += points[p].values[s];//Add the value to the current sum for that particular dimension
            }
            sums[current_cluster] = current_values;//Set the sums vector at the current cluster equal to the new values
        }
        vector<Point> new_centroids;//Will hold the new centroids
        for(int f = 0; f < num_clusters; f++)//For every cluster
        {
            if(counts[f] != 0)
            {
                vector<double> new_centroid_coords; //Store the coordinates for the new centroid for the current cluter
                vector<double> cluster_values = sums[f];//Get the summed values for the current cluster.
                for(int w = 0; w < cluster_values.size(); w++)//For dimension in the summed value vector
                {
                    //The new coordinate for the current dimension is equal to the summed value divided by the number of points in that cluster
                    double new_coord = cluster_values[w] / counts[f];  
                    new_centroid_coords.push_back(new_coord); //Add this new coordinate to the vector of coordinates for the centroid
                }
                new_centroids.push_back(Point(new_centroid_coords, f));//Add a new point with the new cluster location and the cluster number to the list of new centroids
            }
            else
            {
                new_centroids.push_back(centroids[f]);
            }
        }
        centroids = new_centroids;//Set centroids = to the updated list of centroids
    }
    
    pair<vector<Point>, vector<Point>> pair1;
    pair1.first = points;
    pair1.second = centroids;
    return pair1;
    
}


/*pair<vector<Point>, vector<Point>> kmeans_clustering_with_convergence(vector<Point> points, int dimensions, int num_clusters, double convergence_limit)
{
    //Initialize random set of centroids/means
    vector<Point> centroids;
    for (int i = 0; i < num_clusters; i++)
    {
        vector<double> coordinates;
        for (int j = 0; j < dimensions; j++)
        {
            double value = rand()/(double)RAND_MAX;
            coordinates.push_back(value);
        }
        centroids.push_back(Point(coordinates, i));
    }
    //Classify every point into a cluster
    for (int i = 0; i < points.size(); i++)
    {
        double min_distance = DBL_MAX;
        double min_cluster;
        for (int j = 0; j < centroids.size(); j++)
        {
            double cur_distance = distance(points[i], centroids[j]);
            if(cur_distance < min_distance)
            {
                min_distance = cur_distance;
                min_cluster = j;
            }
        }
        points[i].cluster = min_cluster;
    }

    while(cur_variation > convergence_limit)
    {
        //Recompute centroids
        vector<vector<double>> sums;//Stores sum of all individual coordinates for every cluster
        vector<int> counts;//Store number of points in each cluster
        for(int cluster = 0; cluster < num_clusters; cluster++)//For every cluster
        {
            //Create a vector to store the sum for that cluster and start all the values at 0
            vector<double> store_values;
            for(int z = 0; z < dimensions; z++)
            {
                store_values.push_back(0.0);
            }
            sums.push_back(store_values);
            //Start the count for that cluster at 0
            counts.push_back(0);
        }
        for (int p = 0; p < points.size(); p++)//For every point
        {
            int current_cluster = points[p].cluster;//Get its cluster
            counts[current_cluster] += 1; //Increase the count at that cluster by 1
            vector<double> current_values = sums[current_cluster]; //Get the current sums of every coord. at that cluster
            for(int s = 0; s < points[p].values.size(); s++)//For every coord in the current point
            {
                current_values[s] += points[p].values[s];//Add the value to the current sum for that particular dimension
            }
            sums[current_cluster] = current_values;//Set the sums vector at the current cluster equal to the new values
        }
        vector<Point> new_centroids;//Will hold the new centroids
        for(int f = 0; f < num_clusters; f++)//For every cluster
        {
            if(counts[f] != 0)
            {
                vector<double> new_centroid_coords; //Store the coordinates for the new centroid for the current cluter
                vector<double> cluster_values = sums[f];//Get the summed values for the current cluster.
                for(int w = 0; w < cluster_values.size(); w++)//For dimension in the summed value vector
                {
                    //The new coordinate for the current dimension is equal to the summed value divided by the number of points in that cluster
                    double new_coord = cluster_values[w] / counts[f];  
                    new_centroid_coords.push_back(new_coord); //Add this new coordinate to the vector of coordinates for the centroid
                }
                new_centroids.push_back(Point(new_centroid_coords, f));//Add a new point with the new cluster location and the cluster number to the list of new centroids
            }
            else
            {
                new_centroids.push_back(centroids[f]);
            }
        }
        centroids = new_centroids;//Set centroids = to the updated list of centroids
    }
    
    pair<vector<Point>, vector<Point>> pair1;
    pair1.first = points;
    pair1.second = centroids;
    return pair1;
}

double calc_variation(vector<Point> centroids, vector<Point> points, int dimensions, int num_clusters)
{
    
}*/

// Calculating distance 
double distance(Point p1, Point p2) 
{ 
    double sum = 0.0;
    for (int coord = 0; coord < p1.values.size(); coord++)
    {
        sum += pow(p2.values[coord] - p1.values[coord], 2);
    }
    return sqrt(sum * 1.0); 
} 

void createPoint(int x1, int y1, int cluster, vector<vector<int>> &picture)
{
    if(cluster == 0)
    {
        picture[x1][y1] = 1;
    }
    if(cluster == 1)
    {
        picture[x1][y1] = 2;
    }
    if(cluster == 2)
    {
        picture[x1][y1] = 3;
    }
    if(cluster == 3)
    {
        picture[x1][y1] = 4;
    }
    if(cluster == 4)
    {
        picture[x1][y1] = 5;
    }
    if(cluster == 5)
    {
        picture[x1][y1] = 6;
    }
        
    if (x1 > 0)
    {
        if(cluster == 0)
        {
            picture[x1-1][y1] = 1;
        }
        if(cluster == 1)
        {
            picture[x1-1][y1] = 2;
        }
        if(cluster == 2)
        {
            picture[x1-1][y1] = 3;
        }
        if(cluster == 3)
        {
            picture[x1-1][y1] = 4;
        }
        if(cluster == 4)
        {
            picture[x1-1][y1] = 5;
        }
        if(cluster == 5)
        {
            picture[x1-1][y1] = 6;
        }
    }
    if (x1 < 799)
    {
        if(cluster == 0)
        {
            picture[x1+1][y1] = 1;
        }
        if(cluster == 1)
        {
            picture[x1+1][y1] = 2;
        }
        if(cluster == 2)
        {
            picture[x1+1][y1] = 3;
        }
        if(cluster == 3)
        {
            picture[x1+1][y1] = 4;
        }
        if(cluster == 4)
        {
            picture[x1+1][y1] = 5;
        }
        if(cluster == 5)
        {
            picture[x1+1][y1] = 6;
        }
    }
    if (y1 > 0)
    {
        if(cluster == 0)
        {
            picture[x1][y1-1] = 1;
        }
        if(cluster == 1)
        {
            picture[x1][y1-1] = 2;
        }
        if(cluster == 2)
        {
            picture[x1][y1-1] = 3;
        }
        if(cluster == 3)
        {
            picture[x1][y1-1] = 4;
        }
        if(cluster == 4)
        {
            picture[x1][y1-1] = 5;
        }
        if(cluster == 5)
        {
            picture[x1][y1-1] = 6;
        }
    }
    if (y1 < 799)
    {
        if(cluster == 0)
        {
            picture[x1][y1+1] = 1;
        }
        if(cluster == 1)
        {
            picture[x1][y1+1] = 2;
        }
        if(cluster == 2)
        {
            picture[x1][y1+1] = 3;
        }
        if(cluster == 3)
        {
            picture[x1][y1+1] = 4;
        }
        if(cluster == 4)
        {
            picture[x1][y1+1] = 5;
        }
        if(cluster == 5)
        {
            picture[x1][y1+1] = 6;
        }
    }
    if (x1 > 0 && y1 > 0)
    {
        if(cluster == 0)
        {
            picture[x1-1][y1-1] = 1;
        }
        if(cluster == 1)
        {
            picture[x1-1][y1-1] = 2;
        }
        if(cluster == 2)
        {
            picture[x1-1][y1-1] = 3;
        }
        if(cluster == 3)
        {
            picture[x1-1][y1-1] = 4;
        }
        if(cluster == 4)
        {
            picture[x1-1][y1-1] = 5;
        }
        if(cluster == 5)
        {
            picture[x1-1][y1-1] = 6;
        }
    }
    if (x1 < 799 && y1 > 0)
    {
        if(cluster == 0)
        {
            picture[x1+1][y1-1] = 1;
        }
        if(cluster == 1)
        {
            picture[x1+1][y1-1] = 2;
        }
        if(cluster == 2)
        {
            picture[x1+1][y1-1] = 3;
        }
        if(cluster == 3)
        {
            picture[x1+1][y1-1] = 4;
        }
        if(cluster == 4)
        {
            picture[x1+1][y1-1] = 5;
        }
        if(cluster == 5)
        {
            picture[x1+1][y1-1] = 6;
        }
    }
    if (x1 > 0 && y1 < 799)
    {
        if(cluster == 0)
        {
            picture[x1-1][y1+1] = 1;
        }
        if(cluster == 1)
        {
            picture[x1-1][y1+1] = 2;
        }
        if(cluster == 2)
        {
            picture[x1-1][y1+1] = 3;
        }
        if(cluster == 3)
        {
            picture[x1-1][y1+1] = 4;
        }
        if(cluster == 4)
        {
            picture[x1-1][y1+1] = 5;
        }
        if(cluster == 5)
        {
            picture[x1-1][y1+1] = 6;
        }
    }
    if (x1 > 0 && y1 < 799)
    {
        if(cluster == 0)
        {
            picture[x1+1][y1+1] = 1;
        }
        if(cluster == 1)
        {
            picture[x1+1][y1+1] = 2;
        }
        if(cluster == 2)
        {
            picture[x1+1][y1+1] = 3;
        }
        if(cluster == 3)
        {
            picture[x1+1][y1+1] = 4;
        }
        if(cluster == 4)
        {
            picture[x1+1][y1+1] = 5;
        }
        if(cluster == 5)
        {
            picture[x1+1][y1+1] = 6;
        }
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
            if(picture[i][j] == 1)
            {
                ofs << 252 << " " << 232 << " " << 3 << " ";
            }
            else if(picture[i][j] == 2)
            {
                ofs << 235<< " " << 134 << " " << 52 << " ";
            }
            else if(picture[i][j] == 3)
            {
                ofs << 255 << " " << 255 << " " << 255 << " ";
            }
            else if(picture[i][j] == 4)
            {
                ofs << 52 << " " << 235 << " " << 201 << " ";
            }
            else if(picture[i][j] == 5)
            {
                ofs << 0 << " " << 0 << " " << 255 << " ";
            }
            else if(picture[i][j] == 6)
            {
                ofs << 255 << " " << 0 << " " << 0 << " ";
            }
            else
            {
                ofs << 0 << " " << 0 << " " << 0 << " ";
            }      
        }
        ofs << endl;
    }
}