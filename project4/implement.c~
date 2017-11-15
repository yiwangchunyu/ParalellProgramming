#include <stdio.h>
#include <string.h>
#include <mpi.h>

double f(double x)
{
	return x*x;
}

//Trap函数用来计算小梯形的面积
double Trap(double left_endpt,double right_endpt,int trap_count,double base_len)
{
	double estimate,x;
	int i;
	estimate=(f(left_endpt)+f(right_endpt))/2.0;
	for(i=1;i<=trap_count-1;i++)
	{
		x=left_endpt+i*base_len;
		estimate+=f(x);
	}
	estimate=estimate*base_len;
	return estimate;
}
int main()
{
	int my_rank,comm_sz,n=1024,local_n,rest;
	double a=0.0,b=3.0,h,local_a,local_b;
	double local_int,total_int;
	int source;
	double recv;
	int t;
	MPI_Init(NULL,NULL);
	MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
	MPI_Comm_size(MPI_COMM_WORLD,&comm_sz);
	
	MPI_Barrier(MPI_COMM_WORLD);
	double start = MPI_Wtime();
	for(t=0;t<1000;t++)
	{
	h=(b-a)/n;//将横坐标分成1024份，h为每一小分的长度
	local_n=n/comm_sz;//local_n为一个进程应该计算多少份长度的横坐标
	rest=n%comm_sz;//rest表示不能等分时，剩下的单位
	
	local_a=a+my_rank*local_n*h;//计算的初始坐标
	local_b=local_a+local_n*h;//计算的终止坐标

	if(my_rank==comm_sz-1)
		local_int=Trap(local_a,b,local_n+n%comm_sz,h);
	else
	local_int=Trap(local_a,local_b,local_n,h);//计算当前区域的面积
	
//	if(my_rank!=0)
//	{
//		MPI_Send(&local_int,1,MPI_DOUBLE,0,0,MPI_COMM_WORLD);
//	}
//	else
//	{
//		total_int=local_int;
//		for(source=1;source<comm_sz;source++)
//		{
//			MPI_Recv(&local_int,1,MPI_DOUBLE,source,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
//			total_int+=local_int;
//		}
//		
//	}
//	MPI_Allreduce(&local_int, &total_int, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);	
	int times;
	int i;
	for(times=0;pow(2,times)<comm_sz;times++);
	for(i=0;i<times;i++)
	{
		int pow=1,j;
		for(j=0;j<=i;j++)
			pow=pow*2;
		if(my_rank%pow==pow/2)
		MPI_Send(&local_int,1,MPI_DOUBLE,my_rank-pow/2,0,MPI_COMM_WORLD);
		if(my_rank%pow==0)
		{
		MPI_Recv(&recv, 1, MPI_DOUBLE, MPI_ANY_SOURCE,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		local_int+=recv;
		}
		
	}
	MPI_Bcast(&local_int,1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	}
	double end = MPI_Wtime();
	if(my_rank==0)
	{
		printf("time: %lf ms\n", (end-start)/1000);
		printf("with n = %d trapezoids,our estimate\n",n);
		printf("of the integral from %f to %f = %.15e\n",a,b,local_int);
	}
	
	MPI_Finalize();
	
	return 0;
}
