#include "dbscan.h"
#include <math.h>
#include <queue>
#include<iostream>
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
using namespace std;

/*
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
*/

unsigned int total=0;
__global__ void queryKernel(double *dev_nodeX, double *dev_nodeY, int *dev_result,int* dev_p,double* dev_eps)  
{  
    int i = threadIdx.x;  
	dev_result[i]=1;
	if(i!=*dev_p)
	{
		//int dist=euclidean_distance(dev_nodeX[i],dev_nodeY[i],dev_nodeX[*dev_p],dev_nodeY[*dev_p]);
		double x=dev_nodeX[i]-dev_nodeX[*dev_p];
		double y=dev_nodeY[i]-dev_nodeY[*dev_p];
		int dist=sqrt(x*x+y*y);
		if(dist<*dev_eps)
		{
			dev_result[i]=-9999;
		}
	}
}  

int region_query( vector<Point> &dataset, int p, queue<int> &neighborhood, double eps)
{
	int size=dataset.size();
	
	double* dev_nodeX;
	double* dev_nodeY;
	int* dev_result;
	int* dev_p;
	double* dev_eps;
	
	int *result=(int*)malloc(size*sizeof(int));
	double *nodeX = (double*)malloc(size*sizeof(double));
	double *nodeY = (double*)malloc(size*sizeof(double));
	int i;
	for(i=0;i<size;i++)
	{
		nodeX[i]=dataset[i].x;
		nodeY[i]=dataset[i].y;
	}
	
	cudaMalloc((void**)&dev_nodeX,size*sizeof(double));
	cudaMalloc((void**)&dev_nodeY,size*sizeof(double));
	cudaMalloc((void**)&dev_result,size*sizeof(int));
	cudaMalloc((void**)&dev_p,sizeof(int));
	cudaMalloc((void**)&dev_eps,sizeof(double));
	cudaMemcpy(dev_nodeX,nodeX,size*sizeof(double),cudaMemcpyHostToDevice);
	cudaMemcpy(dev_nodeY,nodeY,size*sizeof(double),cudaMemcpyHostToDevice);
	cudaMemcpy(dev_p,&p,sizeof(int),cudaMemcpyHostToDevice);
	cudaMemcpy(dev_eps,&eps,sizeof(double),cudaMemcpyHostToDevice);
	
	//printf("step into kernel\n");
	queryKernel<<<1,size>>>(dev_nodeX,dev_nodeY,dev_result,dev_p,dev_eps); 
	//printf("step out of kernel\n");
	
	cudaThreadSynchronize();
	cudaMemcpy(result,dev_result,size*sizeof(int),cudaMemcpyDeviceToHost);
	
	//printf("%u:",total++);
	for(i=0;i<size;i++)
	{
		if(result[i]==-9999)
		{
			neighborhood.push(i);
			//printf("%d ",i);
		}
			
	}
	//printf("\n");
	
	free(result);free(nodeX);free(nodeY);
	cudaFree(dev_nodeX);cudaFree(nodeY);cudaFree(dev_result);cudaFree(dev_p);cudaFree(dev_eps);
	
	return (int)neighborhood.size();
}
// expand cluster formed by p, which works in a way of bfs.
bool expand_cluster( vector<Point> &dataset, int p, int c, double eps, int min_pts){
    queue<int> neighbor_pts;
    dataset[p].lable = c;
	
    //printf("step into query1\n");
    region_query(dataset, p, neighbor_pts, eps);
	//printf("step out of query1\n");
    
    while (!neighbor_pts.empty()) {
        
        int neighbor = neighbor_pts.front();
        queue<int> neighbor_pts1;
		
		//printf("neighbor is %d\n",neighbor);
		//printf("step into query2\n");
        region_query(dataset, neighbor, neighbor_pts1, eps);
        //printf("step out of query2\n");
		
		
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
	int count = 0;
	int p;
    for (p = 0; p<dataset.size(); p++) {
        queue<int> neighborhood;
		printf("%d query start\n",count++);
        region_query(dataset, p, neighborhood, eps);//找到p的相邻节点，它们之间的距离小于eps，将相邻节点存放到队列中
        //printf("%d query end\n",count);
        if (neighborhood.size()+1 < min_pts) {//如果p相离节点与其组成团的大小 小于最小的要求，将其标记为0
            // mark as noise
			printf("miaomiaomiao?");
            dataset[p].lable = 0;
        }else
        {
           
            if(dataset[p].lable==-1){//否则，如果p没有被分类，则将其相邻节点扩展
                 c++;
				 //printf("step into cluster\n");
                expand_cluster(dataset, p,c, eps, min_pts);
				//printf("step out of cluster\n");
            }
        }
    }
    return c;
    
}