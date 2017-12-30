#pragma once
struct Point {
	double x;
	double y;
	int label;  // -1 unvisited, 0 noise, >0 cluster index
};