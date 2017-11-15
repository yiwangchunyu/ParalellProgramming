#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

int thread_count;

void* Hello(void* rank);

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

void main(int argc, char* argv[])
{
	long thread;
	pthread_t* thread_handles;

	thread_count = strtol(argv[1], NULL, 10);
	thread_handles = malloc(thread_count*sizeof(pthread_t));

	for(thread=0;thread<thread_count;thread++)
		pthread_create(&thread_handles[thread],NULL, Hello, (void*)thread);
	printf("Hello from the main thread\n");
	for(thread=0; thread<thread_count; thread++)
		pthread_join(thread_handles[thread],NULL);
	free(thread_handles);
	//return NULL;
}
void *Hello(void* rank)
{
	long my_rank = (long)rank;
	printf("Hello from thread %ld of %d\n", my_rank, thread_count);
	//return NULL;
}
