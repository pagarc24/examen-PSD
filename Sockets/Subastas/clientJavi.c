int main(){

    serverIP = argv[1];
    port = atoi(argv[2]);
    socketfd = socket(...);

    //Inicializamos la estructura de la direccion del servidor

    connect(socketfd, ...);

    

    recv(socketfd, &code, sizeof(code), 0);
    if (code == CONNECTION_OK){
        char message[256];                                              //pedimos nombre
        printf("Enter your name: ");
        memset(message, 0, 256);
        fgets(message, 256-1, stdin);

        send(socketfd, message, strlen(message), 0);

        recv(socketfd, &infodescription, STRING_LENGTH, 0);
        recv(socketfd, &infocost, sizeof(infocost), 0);

        printf("Introduce puja");
        scanf(puja);
        send(socketfd, puja, sizeof(puja), 0);

        recv(socketfd, &code, sizeof(code), 0);
        if(code == BID_WIN)
            printf("Has gando");
        else
            printf("Perdiste");
    }
    else
        printf("Error server full  o no \n");

}