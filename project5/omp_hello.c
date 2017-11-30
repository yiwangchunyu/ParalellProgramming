#include<stdio.h>
#include<stdlib.h>
#include<omp.h>

int main(int argc, char* argv[])
{
	
	int thread_count=strtol(argv[1], NULL, 10);
	double sum=0.0;
	int n=10;
	int i;
#	pragma omp paralell for num_threads(thread_count)\
		reduction(+:sum) schedule(static,1)
	for(i=0;i<=n;i++)
	{
		sum+=i;
		
		printf("%d of %d\n",omp_get_thread_num(), omp_get_num_threads());
		
	}
	
	return 0;
}


