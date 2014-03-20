#include <stdio.h>

double monte_carlo_slice(int start, int end, t_func func);

typedef double (*t_func) (double x);

int main(int argc, char *argv)
{

	int myid, numprocs;

	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD,&myid);

	printf ("%d of %d: Hello world!\n", myid, numprocs);

	MPI_Finalize();


	return 0;

}

double monte_carlo_slice(int start, int end, t_func func)
{

	int i;
	double sum = 0;
	for(i=start;i<end;i++)
	{
		sum += func(i);
	}

}
