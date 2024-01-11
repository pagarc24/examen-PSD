#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <pthread.h>

/** Maximum number of connections */
#define MAX_CONNECTIONS 4
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
#define STRING_LENGTH 128

typedef struct tSala{
  int socket_server;
  int client1_socketfd;
  int client2_socketfd;
} tSala;

typedef struct tCasino{
    tSala sala[MAX_ROOMS];
} tCasino;

int analisisDePujas (int precioInicial, int puja1, int puja2){
    int winner;

    if(puja1>precioInicial || puja2>precioInicial){
        if(puja1>puja2) winner = 1;
        else if (puja2 > puja1) winner = 2;
        else winner = 0;
    } else winner = 0;

    return winner;
}

void *logicaThread(void *args){
    tSala *sala;
    sala = (tSala *) args;
    int socket_client1 = sala->client1_socketfd;
    int socket_client2 = sala->client2_socketfd;
    int socketfd = sala->socket_server;
    int code, msgLength;
    tString description = "Batidora con la cara de Messi", p1, p2;
    int prize = 100, puja1, puja2;

    code = CONNECTION_OK;
    send(socket_client1, &code, sizeof(code), 0);
    send(socket_client2, &code, sizeof(code), 0);

    //Recv nombres
    memset(p1, 0, STRING_LENGTH);
    recv(socket_client1, p1, STRING_LENGTH-1, 0);
    printf("Jug1: %s\n", p1);

    memset(p2, 0, STRING_LENGTH);
    recv(socket_client2, p2, STRING_LENGTH-1, 0);
    printf("Jug2: %s\n", p2);

    //msgLength = strlen(description)-1;
    msgLength = strlen(description);
    
    send(socket_client1, &msgLength, sizeof(msgLength), 0);
    send(socket_client1, &description, msgLength, 0);
    send(socket_client1, &prize, sizeof(prize), 0);

    send(socket_client2, &msgLength, sizeof(msgLength), 0);
    send(socket_client2, &description, msgLength, 0);
    send(socket_client2, &prize, sizeof(prize), 0);

    printf("\nRESULTADOS DE LA PUJA POR %s(%d)\n\n", description, prize);
    recv(socket_client1, &puja1, sizeof(puja1), 0);
    printf("%s -> %d\n", p1, puja1);
    recv(socket_client2, &puja2, sizeof(puja2), 0);
    printf("%s -> %d\n", p2, puja2);

    int winner = analisisDePujas(prize, puja1, puja2);//0 si no gana nadie, 1 si gana p1, 2 si gana p2
    
    if(winner==0){
        code = BID_LOSE;
        send(socket_client1, &code, sizeof(code), 0);
        send(socket_client2, &code, sizeof(code), 0);
    } else if (winner == 1){
        code = BID_WIN;
        send(socket_client1, &code, sizeof(code), 0);
        code = BID_LOSE;
        send(socket_client2, &code, sizeof(code), 0);
    } else if (winner == 2){
        code = BID_LOSE;
        send(socket_client1, &code, sizeof(code), 0);
        code = BID_WIN;
        send(socket_client2, &code, sizeof(code), 0);
    }

    return 0;

}

unsigned int salaLibre(tCasino c){
    unsigned int i;
    for(i= 0; i < MAX_ROOMS;i++){
        if(c.sala[i].client1_socketfd==-1 ||c.sala[i].client2_socketfd==-1)
            return i;
    }
}

tCasino initCasino(){
    tCasino c;
    unsigned int i;
    for (i= 0; i < MAX_ROOMS; i++){
        c.sala[i].client1_socketfd=-1;
        c.sala[i].client2_socketfd=-1;
    }
    return c;
}

int main(int argc, char *argv[]){
    int socketfd, port, conexiones = 0, socketCliente, socketDesechar, clientLength, code;
    struct sockaddr_in server_add;
    struct sockaddr_in client_add;
    tCasino casino = initCasino();
    unsigned int iSala;
    pthread_t hilos[MAX_ROOMS];

    socketfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    port = atoi(argv[1]);

    memset(&server_add, 0, sizeof(server_add));
    server_add.sin_addr.s_addr = htonl(INADDR_ANY);
    server_add.sin_family = AF_INET;
    server_add.sin_port = htons(port);

    bind(socketfd, (struct sockaddr*) &server_add, sizeof(server_add));

    listen(socketfd, MAX_CONNECTIONS);

    while (TRUE){
        if (conexiones<MAX_CONNECTIONS){
            iSala = salaLibre(casino);
            casino.sala[iSala].client1_socketfd=accept(socketfd, (struct sockaddr*) &client_add, &clientLength);
            printf("%d\n", casino.sala[iSala].client1_socketfd);
            
            casino.sala[iSala].client2_socketfd=accept(socketfd, (struct sockaddr*) &client_add, &clientLength);
            printf("%d\n", casino.sala[iSala].client2_socketfd);
            casino.sala[iSala].socket_server = socketfd;
            pthread_create(&hilos[iSala], NULL, logicaThread, &casino.sala[iSala]);
            
            conexiones +=2;
        } else{
            code = SERVER_FULL;
            socketDesechar = accept(socketfd, (struct sockaddr*) &client_add, &clientLength);
            send(socketDesechar, code, sizeof(code), 0);
            close(socketDesechar);
        }
    }

    for (int i=0; i < iSala; i++){
		pthread_join(hilos[i], NULL);
		exit(0);
        close(casino.sala[i].client1_socketfd);
        shutdown(casino.sala[i].client1_socketfd,2);
        casino.sala[i].client1_socketfd=-1;
        close(casino.sala[i].client1_socketfd);
        shutdown(casino.sala[i].client2_socketfd,2);
        casino.sala[i].client2_socketfd=-1;
        conexiones-=2;
	}

    return 0;
}