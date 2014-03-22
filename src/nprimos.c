#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

int myid, numprocs;
char processor_name[MPI_MAX_PROCESSOR_NAME];
int name_len;


int dividers(int n);


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
    int total = 0;

    int count_per_rank=0, count_global=0;

    // printf("[%d] count_per_rank: %d\n", myid, count_per_rank);

    do {    

        n++;

        int count_per_rank = dividers(n);
        //
        MPI_Allreduce(&count_per_rank, &count_global, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

        // printf("[%d]: count_per_rank: %d, n: %d\n", myid, count_per_rank, n);
        // printf("[%d]: count_global: %d, n: %d\n", myid, count_global, n);
        if(count_global==0) // it's a prime!
        {
            total++;
        }

    } while(total<k);

    if(!myid)
    {
        printf("Primo: %d/%d: %d\n", total, k, n);
    }

    endtime   = MPI_Wtime();
    if(!myid)
        printf("That took %f seconds\n",endtime-starttime);

	MPI_Finalize();
	return 0;
} 


int dividers(int n)
{
    int count = 0;
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

    for(int i = start; i < (start + range); i++)
        if((i > 1)&&(i < n)&&(n % i == 0))
            count++;

    return count;
}
