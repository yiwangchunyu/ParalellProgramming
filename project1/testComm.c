#include<stdio.h>
#include<string.h>
#include<mpi.h>

const int MAX_LEN = 1024;

int main(){
    //char greeting[MAX_STRING];
    char buf[1024];
    
    int size=1;
    int comm_sz;
    int my_rank;
    int i=0;
    double start,end;
   
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    
    for(size=1; size<=1024; size=size*2)
    {
	start = MPI_Wtime();
        for(i=0; i<1000; i++)
        {
            if(my_rank!=0) {
	        MPI_Send(buf, size, MPI_CHAR, 0, i, MPI_COMM_WORLD);
	    }else {
		MPI_Recv(buf, MAX_LEN, MPI_CHAR, MPI_ANY_SOURCE, i, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
        }
        
        end = MPI_Wtime();
	if(my_rank==0)
        {
            printf("It tooks %f second(s), this is process %d, dataSize %d byte\n\n", end - start, my_rank, size);
        }
     }
    MPI_Finalize();
    return 0;
}

