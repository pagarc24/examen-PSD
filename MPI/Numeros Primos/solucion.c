#include "mpi.h"
//Hecho en clase el 12 de diciembre de 2023
void executeMaster(int grain, int numProc){
    int numElements = grain, tag = 1, numSend = 0, numRecv = 0, n;
    MPI_Status status;
    int *vector = malloc(sizeof(int)*V_LEN);
    int *auxPtr;
    for(int i = 1; i < numProc; i++){
        MPI_Send(&numElements, 1, MPI_INT, i, tag, MPI_COMM_WORLD);
        numSend += numElements;
    }
    while(numRecv < V_LEN){
        MPI_Recv(&n, 1, MPI_INT, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &status);
        MPI_Recv(auxPtr, n, MPI_INT, status.MPI_Source, tag, MPI_COMM_WORLD);
        numRecv += n;
        auxPtr += n;
        if(numSend < V_LEN){
            numElements = (V_LEN - numSend) >= grain ? grain : V_LEN - numSend;
            MPI_Send(&numElements, 1, MPI_INT, status.MPI_Source, MPI_COMM_WORLD);
            numSend += numElements;
        }
        else{
            numElements = END;
            MPI_Send(&numElements, 1, MPI_INT, status.MPI_Source, MPI_COMM_WORLD);
        }
    }
}

void executeWorker(int grain){
    int *primos = malloc(sizeof(unsigned short int)*grain);
    int n;
    do{
        MPI_Recv(&n, 1, MPI_INT, MASTER, MPI_COMM_WORLD);
        for(int i = 0; i < n; i++){
            vector[i] = generatePrimeNumber();
        }
        MPI_Send(&n, 1, MPI_INT, status, MPI_SOURCE, MPI_COMM_WORLD);
        MPI_Send(&primos, n, MPI_INT, status, MPI_SOURCE, MPI_COMM_WORLD);
    }while(n!=END);
}
