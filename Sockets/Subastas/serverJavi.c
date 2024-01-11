/** True value */
#define TRUE 1
/** False value */
#define FALSE 0
/** Length for tString */
#define STRING_LENGTH 128
/** Type for names and object description */
typedef char tString [STRING_LENGTH];
/** Maximum number of rooms */
#define MAX_ROOMS 2
/** Server Full */
#define SERVER_FULL 1000
/** Connection OK */
#define CONNECTION_OK 2000
/** Bid win */
#define BID_WIN 3000
/** Bid lose */
#define BID_LOSE 4000

int Fsubasta(void *arg){
    int socketPlayer1;
	int socketPlayer2;
    tString name1, name2;
    tString infodescription;
    int infocost;
    int puja1, puja2;

    struct threadArgs *s = (struct threadArgs *)arg;
    socketPlayer1 = s->socketPlayer1;
	socketPlayer2 = s->socketPlayer2;
    socketPlayer2 = s->infodescription;
    socketPlayer2 = s->infocost;

    send(socketPlayer1, &CONNECTION_OK, sizeof(code1), 0);            //envia a ambos el ok conection
    send(socketPlayer2, &CONNECTION_OK, sizeof(code1), 0);

    recv(currentPlayer1, name1, STRING_LENGTH, 0);                  //espera nombres de ambos
    recv(currentPlayer2, name2, STRING_LENGTH, 0);

    send(socketPlayer1, &infodescription, STRING_LENGTH, 0);        //envia info producto p1
    send(socketPlayer1, &infocost, sizeof(infocost), 0);

    send(socketPlayer2, &infodescription, STRING_LENGTH, 0);        //envia info producto p2
    send(socketPlayer2, &infocost, sizeof(infocost), 0);

    recv(currentPlayer1, puja1, sizeof(puja1), 0);                  //recive puja ambos
    recv(currentPlayer2, puja2, sizeof(puja2), 0);

    if(puja1 > infocost & puja1 > puja2){                           //Mira quien ha ganado y manda veredicto
        send(socketPlayer1, &BID_WIN, sizeof(BID_WIN), 0);
        send(socketPlayer2, &BID_LOSE, sizeof(BID_LOSE), 0);
    }
    else if(puja2 > infocost & puja2 > puja2){
        send(socketPlayer1, &BID_LOSE, sizeof(BID_LOSE), 0);
        send(socketPlayer2, &BID_WIN, sizeof(BID_WIN), 0);
    }
    else{
        send(socketPlayer1, &BID_LOSE, sizeof(BID_LOSE), 0);
        send(socketPlayer2, &BID_LOSE, sizeof(BID_LOSE), 0);
    }

    close(socketPlayer1);
    close(socketPlayer2);

}


int mainServer{
    int conexiones = 0;
    
    port = atoi(argv[1]);
    socketId = socket(...);
    bind(socketId, ...);
    listen(socketId, 4);

    while(1){
        if (conexiones < 4){
            socketPlayer1 = acceptConnection(socketfd);
            conexiones++;

            socketPlayer2 = acceptConnection(socketfd);
            conexiones++;

            struct threadArgs argH;
            argH.socketPlayer1 = socketPlayer1;
            argH.socketPlayer2 = socketPlayer2;
            if(pthread_create(&hilo, NULL, Fsubasta, (void *)&argH) != 0){
                printf("Error al crear el hilo");
            }
        }
        else{
            socketPlayerout = acceptConnection(socketfd);
            send(socketPlayerout, &SERVER_FULL, sizeof(codeG), 0);
            close(socketPlayerout);
        }
    }
}