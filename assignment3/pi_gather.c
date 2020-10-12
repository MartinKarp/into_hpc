
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define SEED     921
#define NUM_ITER 1000000000

int main(int argc, char* argv[])
{
    int count = 0;
    int rank, world_size, provided;
    MPI_Init_thread(&argc, &argv, MPI_THREAD_SINGLE, &provided);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int lcount = 0;
    double x, y, z, pi, t0, t1;
    t0 = MPI_Wtime(); 
    srand(SEED*rank); // Important: Multiply SEED by "rank" when you introduce MPI!
    
    // Calculate PI following a Monte Carlo method
    for (int iter = rank; iter < NUM_ITER; iter+=world_size)
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
    int* count_a = NULL;
    if(rank == 0){
        count_a = (int *) malloc(world_size*sizeof(int));
    }
    MPI_Gather(&lcount,1,MPI_INT,count_a,1,MPI_INT,0,MPI_COMM_WORLD);
    if(rank == 0){
        for(int i = 0; i < world_size; ++i){
            count += count_a[i];
        }
    }
    t1 = MPI_Wtime(); 

    if(rank == 0){
        pi = ((double)count / (double)NUM_ITER) * 4.0;
        printf("The result is %f\n", pi);
        printf("Time passed %f\n", t1-t0);
    }
    MPI_Finalize(); 
    return 0;
}

