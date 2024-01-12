#include "mpi.h"
#define GRAIN 4
#define MAX_NUMBER 15
#define END_OF_PROCESSING -1
#define MASTER 0

void executeMaster(int* array, int numProc)
{
    int sol[9] = {0}, aux[9];
    int numSend = 0, numRecv = 0, i, tag, worker, n;
    MPI_Status status;

    n = GRAIN;
    for(i = 1; i < numProc; i++){
        MPI_Send(n, 1, MPI_INT, i, tag, MPI_COMM_WORLD);
        MPI_Send(&array[numSend], numSend, MPI_INT, i, tag, MPI_COMM_WORLD);
        numSend+=n;
    }

    while(numRecv<MAX_NUMBER){
        MPI_Recv(&n, 1, MPI_INT, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &status);
        worker = status.MPI_SOURCE;
        MPI_Recv(aux, 9, MPI_INT, worker, tag, MPI_COMM_WORLD, &status);
        sol = sumaSolVectores(sol, aux);
        numRecv += n;
        if(numSend < MAX_NUMBER){
            n = (MAX_NUMBER-numSend) >= GRAIN ? GRAIN: MAX_NUMBER-numSend;
            MPI_Send(n, 1, MPI_INT, worker, tag, MPI_COMM_WORLD);
            MPI_Send(&array[numSend], numSend, MPI_INT, worker, tag, MPI_COMM_WORLD);
            numSend+=n;
        } else {
            n = END_OF_PROCESSING;
            MPI_Send(n, 1, MPI_INT, worker, tag, MPI_COMM_WORLD);
        }
    }
    
    pintarSol(sol);
}

void executeworker()
{
    int sol[9] = {0};
    int* aux = malloc(sizeof(int)*GRAIN);
    int n = 0, tag;
    MPI_Status status;

    while (n!=END_OF_PROCESSING)
    {
        MPI_Recv(&n, 1, MPI_INT, MASTER, MPI_COMM_WORLD, &status);
        if(n!=END_OF_PROCESSING){
            MPI_Recv(aux, n, MPI_INT, MASTER, MPI_COMM_WORLD, &status);
            sol = calculaDiv(aux, n);
            MPI_Send(n, 1, MPI_INT, MASTER, tag, MPI_COMM_WORLD);
            MPI_Send(&sol, 9, MPI_INT, MASTER, tag, MPI_COMM_WORLD);
        }
    }
    
}