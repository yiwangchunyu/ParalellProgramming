#pragma once
#ifndef DBSCAN_H
#define DBSCAN_H

#include <vector>

using namespace std;

struct Point {
	double x;
	double y;
	int lable;  // -1 unvisited, 0 noise, >0 cluster index
};
double euclidean_distance(double x1, double y1, double x2, double y2);
int dbscan(vector<Point> &dataset, double eps, int min_pts);

#endif /*DBSCAN_H*/
