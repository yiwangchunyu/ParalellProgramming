#include "dbscan.h"
#include <math.h>
#include <queue>
#include<iostream>
#include <stdio.h>
#include <cstring>
using namespace std;

unsigned int total = 0;
const int maxn = 4005;
int query_result[maxn][maxn];
int datasize;

// calculate eculidean distance of two 2-D points
double euclidean_distance(Point a, Point b)
{
	double x = a.x - b.x;
	double y = a.y - b.y;
	return sqrt(x*x + y*y);
}

// get neighborhood of point p and add it to neighborhood queue
void region_query(vector<Point> &dataset, int p, int query_result[][maxn], double eps, int x, int y)
{
	if (query_result[x][y] != 0) return;
	if (x == y) { query_result[x][y] = query_result[y][x] = -1; return; }
	int dist = euclidean_distance(dataset[x], dataset[y]);

	query_result[x][y] = query_result[y][x] = dist < eps ? 1 : -1;
	return;
}

// expand cluster formed by p, which works in a way of bfs.
bool expand_cluster(vector<Point> &dataset, int p, int c, double eps, int min_pts) {
	vector<int> neighbor_pts;
	//queue<int> neighbor_pts;
	dataset[p].lable = c;

	for (int i = 0; i < datasize; i++)
		if (query_result[p][i] == 1)
			neighbor_pts.push_back(i);
			//neighbor_pts.push(i);
	
	while (neighbor_pts.size() != 0) {
		vector<int> new_neighbor_pts;

# pragma omp parallel for num_threads(200) 
		for (int thread = 0; thread < (int)neighbor_pts.size(); thread++) {
			queue<int> neighbor_pts1;
			int neighbor = neighbor_pts[thread];
			dataset[neighbor].lable = c; 
			for (int i = 0; i < datasize; i++) 
#pragma omp critical
			{
				if (query_result[neighbor][i] == 1)
					neighbor_pts1.push(i);
			}
			
			if ((int)neighbor_pts1.size() >= min_pts - 1) {
				while (!neighbor_pts1.empty()) {
					int pt = neighbor_pts1.front();
					if (dataset[pt].lable == -1) {
						#pragma omp critical
							new_neighbor_pts.push_back(pt);
					}
					neighbor_pts1.pop();
				}
			}
			
			
		}
		neighbor_pts = new_neighbor_pts;
	}
/*---------------------------------
	while (!neighbor_pts.empty()) {

		int neighbor = neighbor_pts.front();
		queue<int> neighbor_pts1;

		for (int i = 0; i < datasize; i++)
			if (query_result[neighbor][i] == 1)
				neighbor_pts1.push(i);

		//region_query(dataset, neighbor, neighbor_pts1, eps);

		if (neighbor_pts1.size() >= min_pts - 1)
		{
			while (!neighbor_pts1.empty()) {

				int pt = neighbor_pts1.front();
				if (dataset[pt].lable == -1) {
					neighbor_pts.push(pt);
				}
				neighbor_pts1.pop();
			}
		}
		dataset[neighbor].lable = c;
		neighbor_pts.pop();
	}*/
	return  true;
}

bool expand_cluster_origin(vector<Point> &dataset, int p, int c, double eps, int min_pts) {
	queue<int> neighbor_pts;
	dataset[p].lable = c;

	for (int i = 0; i < datasize; i++)
		if (query_result[i][p] == 1)
			neighbor_pts.push(i);

	while (!neighbor_pts.empty()) {

		int neighbor = neighbor_pts.front();
		queue<int> neighbor_pts1;
		for (int i = 0; i < datasize; i++)
			if (query_result[neighbor][i] == 1)
				neighbor_pts1.push(i);

		if (neighbor_pts1.size() >= min_pts - 1)
		{
			while (!neighbor_pts1.empty()) {

				int pt = neighbor_pts1.front();
				if (dataset[pt].lable == -1) {
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
	int count = 0;
	datasize = dataset.size();

	memset(query_result, 0, sizeof(query_result));

	int cnt = 0;

# pragma omp parallel for num_threads(datasize)
	for (int i = 0; i < datasize*datasize; i++) {
		region_query(dataset, min_pts, query_result, eps, i / datasize, i%datasize);
	}
	puts("query_result done.");

	for (int p = 0; p<datasize; p++) {

		queue<int> neighborhood;
		for (int i = 0; i < datasize; i++)
			if (query_result[p][i] == 1)
				neighborhood.push(i);

		if (neighborhood.size() + 1 < min_pts) {
			// mark as noise
			printf("miaomiaomiao?");
			dataset[p].lable = 0;
		}
		else
		{
			if (dataset[p].lable == -1) {
				c++;
				expand_cluster(dataset, p, c, eps, min_pts);
			}
		}
	//	printf("%d done\n", p);
	}
	return c;

}
