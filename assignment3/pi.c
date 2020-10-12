#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <mpi.h>
#include <time.h>

#define SEED     921
#define NUM_ITER 1000000000

int main(int argc, char* argv[])
{
    int count = 0;
    double x, y, z, pi;
    int rank, size, i, provided;
    MPI_Init_thread(&argc, &argv, MPI_THREAD_SINGLE, &provided);
    double start_time, stop_time, elapsed_time;
    start_time = MPI_Wtime();

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int lcount = 0;
   
    srand(rank*SEED); // Important: Multiply SEED by "rank" when you introduce MPI!
    
    // Calculate PI following a Monte Carlo method
    for (int iter = rank; iter < NUM_ITER; iter+=size)
    {
        // Generate random (X,Y) points
        x = (double)random() / (double)RAND_MAX;
        y = (double)random() / (double)RAND_MAX;
        z = sqrt((x*x) + (y*y));
        
        // Check if point is in unit circle
        if (z <= 1.0)
        {
            lcount++;
        }
    }
    if (rank == 0){
	int counts[size-1];
	for (i = 1; i < size; i++){
		MPI_Recv(&counts[i-1], 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}
	count += lcount;
	for (i = 1; i < size; i++){
		count += counts[i-1];
	}

    	// Estimate Pi and display the result
    	pi = ((double)count / (double)NUM_ITER) * 4.0;
    } else {

    	MPI_Send(&lcount,1,MPI_INT,0,0,MPI_COMM_WORLD);
    } 
    stop_time = MPI_Wtime();
    elapsed_time = stop_time - start_time;
    if (rank == 0){
    	printf("The result is %f\n", pi);
    	printf("Execution time %f\n", elapsed_time);
    }
    
    MPI_Finalize();
    return 0;
}

