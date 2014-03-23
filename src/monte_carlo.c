#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

int myid, numprocs;
char processor_name[MPI_MAX_PROCESSOR_NAME];
int name_len;

/*
*  range_reduce: It will divide n as good as possible to balance the clusters
*/
void range_reduce(long int n, long int *start_ptr, long int *range_ptr);

/*
* Math functions
*/

/*
* Generate random numbers using the uniform distribution
*/
double uniform(double a, double b);

/*
* Custom type for discrete functions of one variable
*/
typedef double (*t_func) (double x);

/*
* Monte carlo simplification for per cluster usage
*/
long double monte_carlo_slice(double min, double max, t_func func, long int samples);

/*
* Test functions
*/
double linear(double x);
double cubic_plus_quad(double x);
double linear2(double x);

int main(int argc, char *argv[])
{

	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD,&myid);

    // Get the name of the processor
    MPI_Get_processor_name(processor_name, &name_len);

	printf ("[%s] %d of %d: starting...\n", processor_name, myid, numprocs);

    // get the min and max limits 
    double min = atof(argv[1]);
    double max = atof(argv[2]);
    long int samples = atol(argv[3]);

    long int samples_per_rank;
    range_reduce(samples, NULL, &samples_per_rank);

    long double sum_per_rank=0, sum_global=0, result=0;

    // starts the timer
    double starttime, endtime;
    starttime = MPI_Wtime();
   
    // sum_per_rank = monte_carlo_slice(min, max, linear, samples_per_rank);
    // sum_per_rank = monte_carlo_slice(min, max, linear2, samples_per_rank);
    sum_per_rank = monte_carlo_slice(min, max, cubic_plus_quad, samples_per_rank);

    // printf("[%d] sum_per_rank: %Le\n", myid, sum_per_rank); 
  
    MPI_Allreduce(&sum_per_rank, &sum_global, 1, MPI_LONG_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

    // reduce?
    result = ( max - min  ) * ( sum_global / samples );
   
    endtime   = MPI_Wtime();

    if(!myid)
    {
        printf("Resultado: %Le\n", result);
    }

    if(!myid)
        printf("That took %f seconds\n",endtime-starttime);

	MPI_Finalize();
	return 0;

}

void range_reduce(long int n, long int *start_ptr, long int *range_ptr)
{
    long int range = 0;
    long int start = 2;
    
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

long double monte_carlo_slice(double min, double max, t_func func, long int samples)
{

    long int i=0;
    long double sum = 0.0;
    double x;
    while(i<samples)
    {
        x = uniform(min, max);
        sum += func(x);

//        printf("x: %f, func: %f\n", x, func(x));

        i+=1;
    }

    return sum;

}

double uniform(double a, double b)
{
    srand (time(NULL)*rand());
    return rand() / (RAND_MAX + 1.0) * (b - a) + a;
}

double linear(double x)
{

    return 2.0*x;

}

double linear2(double x)
{

    return 3.0*x;

}

double cubic_plus_quad(double x)
{

    return x*x*x + 2*x*x;

}
