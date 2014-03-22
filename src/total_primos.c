#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

int myid, numprocs;
char processor_name[MPI_MAX_PROCESSOR_NAME];
int name_len;


/*
* test_prime: It will return 1 if n is a prime number, 0 otherwise.
*/
int test_prime(int n);

/*
*  range_reduce: It will divide n as good as possible to balance the clusters
*/
void range_reduce(int n, int *start_ptr, int *range_ptr);


int main( int argc, char *argv[] )
{

	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD,&myid);

    // Get the name of the processor
    MPI_Get_processor_name(processor_name, &name_len);

	printf ("[%s] %d of %d: starting...\n", processor_name, myid, numprocs);

    double starttime, endtime;
    starttime = MPI_Wtime();

	int k = atoi(argv[1]);  
    int n=1;

    int count_per_rank=0, count_global=0;

    int start, range;
    range_reduce(k, &start, &range);

    int i;
    for(i = start; i < (start + range); i++)
    {
        if( test_prime(i) )
        {
            count_per_rank++;
            printf("primo: %d\n", i);
        }
    }

    // printf("[%d] count_per_rank: %d\n", myid, count_per_rank);

    MPI_Allreduce(&count_per_rank, &count_global, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    // printf("[%d]: count_per_rank: %d, n: %d\n", myid, count_per_rank, n);
    // printf("[%d]: count_global: %d, n: %d\n", myid, count_global, n);

    endtime   = MPI_Wtime();

    if(!myid)
    {
        printf("Primos totais: %d\n", count_global);
    }

    if(!myid)
        printf("That took %f seconds\n",endtime-starttime);

	MPI_Finalize();
	return 0;
} 

int test_prime(int n)
{
    if(n<2) return 0;

    int i;
    for(i=2;i<n;i++)
    {
        if( n % i == 0 )
            return 0;
    }

    return 1;

}

void range_reduce(int n, int *start_ptr, int *range_ptr)
{
    int range = 0;
    int start = 2;
    
    if(n < numprocs)
    {
        if(myid < n) range = 1;
    } else {
	range = (n % 2 == 0 ? n : (n + 1)) / numprocs;

        if((myid == 0)&&(n % 2 == 1)) {
            if(range * numprocs < n) range++;
            else if(range * numprocs > n) range--;
        }        
    }

    start = range * myid + 1;

    if(start_ptr)
        *start_ptr = start;

    if(range_ptr)
        *range_ptr = range;

}

