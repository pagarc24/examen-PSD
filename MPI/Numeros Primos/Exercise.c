//Master
int *vector[VECTOR_LENGTH];
int enviados, recividos = 0;

for(int i  = 1 ; i < numProc; i++){
    MPI_Send(grain, 1, MPI_UNSIGNED_SHORT, i, tag, MPI_COMM_WORLD);
    enviados += grain;
}
while (recividos < VECTOR_LENGTH){
    MPI_Recv(&nNum, 1, MPI_INT, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &status);
    MPI_Recv(&vector, 1, MPI_INT, status.MPI_SOURCE, tag, MPI_COMM_WORLD, &status);
    recividos + = nNum;
    vector + = nNum;

    if(enviados < VECTOR_LENGTH){
        if (VECTOR_LENGTH - enviados >= grain)
            int env = grain;
        else
            int env = VECTOR_LENGTH;
        MPI_Send(&env, 1, MPI_UNSIGNED_SHORT, status.MPI_SOURCE, tag, MPI_COMM_WORLD);
        enviados += grain;
    }
    else{
        int end = END_OF_PROCESSING
        MPI_Send(&end, 1, MPI_UNSIGNED_SHORT, status.MPI_SOURCE, tag, MPI_COMM_WORLD);
    }
}

//Worker
vector = (int*) malloc(grain*sizeof(int));
int nNum;
MPI_Recv(&nNum, 1, MPI_INT, MASTER, tag, MPI_COMM_WORLD, &status);
while(nNum != END_OF_PROCESSING){
    for ( int i = 0; i < nNum; i++){
        vector[i] = generatePrimeNumber ();
    }
    MPI_Send(&nNum, 1, MPI_INT, status.MPI_SOURCE, tag, MPI_COMM_WORLD);
    MPI_Send(&vector, 1, MPI_INT, status.MPI_SOURCE, tag, MPI_COMM_WORLD);
    MPI_Recv(&nNum, 1, MPI_INT, status.MPI_SOURCE, tag, MPI_COMM_WORLD, &status);
}
free(vector);