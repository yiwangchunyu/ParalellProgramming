
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>   

double f(int x)
{
	return 1.0*x;
}
void push(int thread_num,int i, int a[][1000])
{
	int *p=a[thread_num];
	int k;
	for(k=0;k<1000&&(*p)!=-1;k++,p++);
		*p=i;
}


/*--------------------------------------------------------------------*/
int main(int argc, char* argv[]) {
	double sum=0;
	int i,k;
	int n=1000;  //the number of the iteration times
	int a[1000][1000]={{-1}};
	for(i=0;i<1000;i++)
		for(k=0;k<1000;k++)
			a[i][k]=-1;
   //int thread_count;
   int thread_count = strtol(argv[1], NULL, 10); 
	//thread_count = omp_get_num_threads();
#  pragma omp parallel for num_threads(thread_count) \
	reduction(+:sum) schedule(runtime) 
//#  pragma omp parallel 
   for(i=0;i<n;i++)
	{
		sum+=f(i);
		
		push(omp_get_thread_num(),i,a);
		
	}

	for(i=0;i<thread_count;i++)
	{
		int j=0;
		printf("thread %d :", i);
		for(j=0;j<1000;j++)
		{
			if(a[i][j]==-1&&j==0) break;
			if(a[i][j]==-1&&j!=0) {printf("%d",a[i][j-1]);break;}
			if(j==0) {printf(" %d-",a[i][j]);continue;}

			if(a[i][j]==a[i][j-1]+1&&j>0) continue;
			if(a[i][j]!=a[i][j-1]+1&&j>0)	
			{
				
				printf("%d %d-",a[i][j-1], a[i][j]);
			}
			
		}
		printf("\n");
	}

   return 0; 
}  /* main */


