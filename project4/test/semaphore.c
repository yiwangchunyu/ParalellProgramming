#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include <sys/time.h>
#include<semaphore.h>

int thread_count,local_n,n=1024;
double total_int=0;
double a=0.0,b=3.0;
double h;
sem_t semaphore;
void* getsum(void* rank);
double Trap(double left_endpt,double right_endpt,int trap_count,double base_len);
double f(double x)
{
	return x*x;
}

int main(int argc, char* argv[])
{
	sem_init(&semaphore, 0, 1);
    struct timeval start;
    struct timeval end;
    gettimeofday(&start,NULL);
	
	long thread;
	int i;
	pthread_t* thread_handles;

	thread_count = strtol(argv[1], NULL, 10);
	thread_handles = malloc(thread_count*sizeof(pthread_t)); 
	for(i=0;i<1000;i++)
	{
		total_int=0;
		//printf("round %d start:\n",i);
		for(thread=0;thread<thread_count;thread++)
			pthread_create(&thread_handles[thread],NULL, getsum, (void*)thread);

		for(thread=0; thread<thread_count; thread++)
			pthread_join(thread_handles[thread],NULL);
	}
	free(thread_handles);
    	gettimeofday(&end,NULL);
	printf("-------semaphore 1,000 times--------\n");
	printf("time: %ld us\n",  1000000 * ( end.tv_sec - start.tv_sec ) + end.tv_usec - start.tv_usec);
		printf("with n = %d trapezoids,our estimate\n",n);
		printf("of the integral from %f to %f = %.15e\n",a,b,total_int);

	return 0;
}
void *getsum(void* rank)
{
	long my_rank = (long)rank;
	int local_n=n/thread_count;
    h=(b-a)/n;
    double res;
	double local_a=a+my_rank*local_n*h;
	double local_b=local_a+local_n*h;

	if(my_rank==thread_count-1)
	{
		res=Trap(local_a,b,local_n+n%thread_count,h);

	}
	else
	{
        res=Trap(local_a,local_b,local_n,h);

    }
	sem_wait(&semaphore);
    total_int+=res;
	sem_post(&semaphore);
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

