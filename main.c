#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <ctype.h>
#include "paragraph.h"
#include "paragraph_list.h"

extern void master();
extern void worker();

int maxThreads, rank;
PARAGRAPH *root;
char in_file[100];
char *types[4] = {"horror\n", "comedy\n", "fantasy\n", "science-fiction\n"};
pthread_mutex_t mutex;
pthread_barrier_t barrier;

int main (int argc, char *argv[]) {
    int numTask, len, provided;
    maxThreads = (int) sysconf(_SC_NPROCESSORS_ONLN);
    MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &provided);
    MPI_Comm_size(MPI_COMM_WORLD, &numTask);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);

    if (numTask != 5) {
        fprintf(stderr, "Wrong number of processes! %d", numTask);
        MPI_Finalize();
        return -1;
    }

    strcpy(in_file, argv[1]);
    if (rank == MASTER)
        master();
    else
        worker();

    MPI_Finalize();
}
