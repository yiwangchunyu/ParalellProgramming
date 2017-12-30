//
//  main.cpp
//  DBSCAN
//
//  Created by Tony SHE on 30/4/15.
//  Copyright (c) 2015 Tony Sheh. All rights reserved.
//

#include <iostream>
#include <fstream>
#include "dbscan.h"
#include "stdlib.h"
#include"time.h"
using namespace std;
int main(int argc, const char * argv[]) {
	clock_t start, finish;
	double duration;
	start = clock();
	vector<Point> dataset;
	ifstream in("./3spiral.txt");
	int i = 0;
	string line;
	if (in) {
		float x, y;
		while (in >> x >> y) {
			Point p;
			p.x = x;
			p.y = y;
			p.lable = -1;
			dataset.push_back(p);
		}
	}
	else {
		cout << "No such file" << endl;
	}
	//printf("total is %d\n",dataset.size());
	int c = dbscan(dataset, 2, 3);//调用处理函数，传入vector
	cout << " （x, y）  cluster" << endl;
	for (int i = 0; i<dataset.size(); i++) {
		cout << "(" << dataset[i].x << "," << dataset[i].y << ") " << dataset[i].lable << endl;
	}
	finish = clock();
	duration = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << duration << "s" << endl;
	system("PAUSE ");
	return 0;
}
