#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

int main( int argc, char *argv[] )
{
	int myid, numprocs;

	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD,&myid);

    // Get the name of the processor
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(processor_name, &name_len);

    int count = 0;

	int n = atoi(argv[1]);
    int range;
    if(n % numprocs)
    {
        range = n / (numprocs-1);
        if(myid == numprocs-1)
        {
            range = n - ( (numprocs-1) * range );
        }
    } else {
        range = n / numprocs;
    }

    // calculo de primo
    int start = ( n / numprocs ) * myid + 1;
    //

	printf ("[%s] %d of %d: %d start:%d\n", processor_name, myid, numprocs, range, start);

    if(myid!=0) // i am a cluster
    {
        MPI_Send(&count, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    } else {
        int sum = count, i;
        for(i=1;i<numprocs;i++)
        {
            MPI_Recv(&count, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            sum += count;
        }
        printf("Divisores: %d\n", sum);
    }

	MPI_Finalize();
	return 0;
} 
