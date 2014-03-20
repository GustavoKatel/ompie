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

    do {    

        n++;

        int count_per_rank = dividers(n);
        //

        MPI_Reduce(&count_per_rank, &count_global, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
        // printf("[%d]: count_per_rank: %d, n: %d\n", myid, count_per_rank, n);
        // printf("[%d]: count_global: %d, n: %d\n", myid, count_global, n);
        if(count_global==0) // it's a prime!
        {
            total++;
        }
      

    } while(total<k);

    if(!myid)
        printf("Primo: %d/%d: %d\n", total, k, n);

    endtime   = MPI_Wtime();
    if(!myid)
        printf("That took %f seconds\n",endtime-starttime);

	MPI_Finalize();
	return 0;
} 


int dividers(int n)
{

    int range = n / numprocs;
    int intComp = 0;
    if(myid==numprocs-1)
    {
        intComp += n - numprocs*range; // compensate the int error
    }

    // prime calculation
    int count = 0;
    int start =  range * myid + 1;

    if(start==1) start++; // skip the first test
    if(start+range>=n) range = n - start;  // skip the last test. BUGFIX: Single rank range error.

    int i;
    for(i=start;i<start+range+intComp;i++)
    {
        if( ! (n%i) )
            count++;
    }

    return count;

}
