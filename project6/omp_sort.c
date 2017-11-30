#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<omp.h>
#include<time.h>
#include<sys/time.h>
#define MAX 100000
int thread_count;
void Count_sort(int a[], int n)
{
	int i,j,count;
	int *temp = malloc(n*sizeof(int));
#	pragma omp parallel for num_threads(thread_count) \
	default(none) shared(a,n,temp) private(i,j,count)
	for(i=0;i<n;i++)
	{
		count=0;
		for(j=0;j<n;j++)
		{
			if(a[j]<a[i])
				count++;
			else if(a[j]==a[i]&&j<i)
				count++;
			
		}
		temp[count]=a[i];
	}
	memcpy(a,temp,n*sizeof(int));
	free(temp);
}


int main(int argc, char *argv[])
{
	thread_count = strtol(argv[1],NULL,10);
	int a[MAX];
	int i;
	for(i=0;i<MAX;i++)
		a[i]=rand()%100;
	struct timeval start;
	struct timeval end;
	gettimeofday(&start,NULL);
	Count_sort(a,MAX);
	gettimeofday(&end,NULL);
	printf("time: %ld us\n", 1000000*(end.tv_sec-start.tv_sec)+end.tv_usec-start.tv_usec);
	//for(i=0;i<MAX;i++)
		//printf("%d ",a[i]);
	putchar(10);
	return 0;
}
