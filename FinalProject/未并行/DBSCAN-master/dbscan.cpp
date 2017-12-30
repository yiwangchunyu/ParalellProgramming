#include "dbscan.h"
#include <math.h>
#include <queue>
#include<iostream>
using namespace std;


// calculate eculidean distance of two 2-D points
double euclidean_distance(Point a, Point b)
{
    double x = a.x-b.x;
    double y = a.y-b.y;
    return sqrt(x*x+y*y);
}

// get neighborhood of point p and add it to neighborhood queue
int region_query( vector<Point> &dataset, int p, queue<int> &neighborhood, double eps)
{
	//int count = 0;
    for (int i = 0; i < dataset.size(); i++) {
		//cout << "regin_query" << count++ << endl;
        if(i!=p){
            int dist = euclidean_distance(dataset[p],dataset[i]);
            if ( dist< eps) {
                neighborhood.push(i);
            }
        }
    }
    return (int)neighborhood.size();
}

// expand cluster formed by p, which works in a way of bfs.
bool expand_cluster( vector<Point> &dataset, int p, int c, double eps, int min_pts){
    queue<int> neighbor_pts;
    dataset[p].lable = c;
    
    region_query(dataset, p, neighbor_pts, eps);
    
    while (!neighbor_pts.empty()) {
        
        int neighbor = neighbor_pts.front();
        queue<int> neighbor_pts1;
        region_query(dataset, neighbor, neighbor_pts1, eps);
        
        if(neighbor_pts1.size()>=min_pts-1)
        {
            while (!neighbor_pts1.empty()) {
                
                int pt = neighbor_pts1.front();
                if(dataset[pt].lable ==-1){
                    neighbor_pts.push(pt);
                }
                neighbor_pts1.pop();
            }
        }
        dataset[neighbor].lable = c;
        neighbor_pts.pop();
        
    }
    return  true;
}

// doing dbscan, given radius and minimum number of neigborhoods.
int dbscan(vector<Point> &dataset, double eps, int min_pts)
{
    int c = 0;  // cluster lable
	//int count = 0;
    for (int p = 0; p<dataset.size(); p++) {
        queue<int> neighborhood;
		//cout << count++ << endl;
        region_query(dataset, p, neighborhood, eps);
        
        if (neighborhood.size()+1 < min_pts) {
            // mark as noise
            dataset[p].lable = 0;
        }else
        {
           
            if(dataset[p].lable==-1){
                 c++;
                expand_cluster(dataset, p,c, eps, min_pts);
            }
        }
    }
    return c;
    
}