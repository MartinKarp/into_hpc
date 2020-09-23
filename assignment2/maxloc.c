#define MAX_THREADS 32

#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>

double mysecond(){
  struct timeval tp;
  struct timezone tzp;
  int i;

  i = gettimeofday(&tp,&tzp);
  return ( (double) tp.tv_sec + (double) tp.tv_usec * 1.e-6 );
}

typedef struct { double val; int loc; char pad[128];} tvals;

int main(int argc, char *argv[]) {
  
    int N = 1000000;
    double x[N];
    srand(time(0)); // seed
  
    for(int i=0; i < N;i++){
    // Generate random number between 0 and 1000
        x[i] = ((double)(rand()) / RAND_MAX)*((double)(rand()) / RAND_MAX)*((double)(rand()) / RAND_MAX)*1000;
    }   

    tvals maxinfo[MAX_THREADS];
    double maxval;
    int maxloc;
    int times = 10;
    double ts = mysecond();
    for (size_t j = 0; j < times; j++){
        maxval = 0.0;
        maxloc = 0;
        #pragma omp parallel shared(maxinfo)
        {
            int id = omp_get_thread_num();
            maxinfo[id].val = -1.0e30;
            #pragma omp for
            for (int i=0; i < N; i++){
                if (x[i] > maxinfo[id].val){
                    maxinfo[id].val = x[i]; 
                    maxinfo[id].loc = i;
                }
            }
        }
        maxval = maxinfo[0].val;
        maxloc = maxinfo[0].loc;
        for(int i = 0; i < MAX_THREADS; i++){
            if (maxinfo[i].val > maxval){
                maxval = maxinfo[i].val; 
                maxloc = maxinfo[i].loc;
            }
        }
    }
    double t = (mysecond() - ts)/times;
    printf("%f\n", t);

    return 0;
}

