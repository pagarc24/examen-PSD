#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

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

int analisisDePujas (int precioInicial, int puja1, int puja2){
    int winner;

    if(puja1>precioInicial || puja2>precioInicial){
        if(puja1>puja2) winner = 1;
        else if (puja2 > puja1) winner = 2;
        else winner = 0;
    } else winner = 0;

    return winner;
}

int main(int argc, char *argv[]){
    int socketfd, port, socket_client1, socket_client2, clientLength, msgLength, puja1, puja2;
    struct sockaddr_in client1_add;
    struct sockaddr_in client2_add;
    struct sockaddr_in server_add;
    char p1[STRING_LENGTH];
    char p2[STRING_LENGTH];
    int winner;
    int code;
    
    tString description = "Batidora con la cara de Messi";
    int prize = 100;

    socketfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    port = atoi(argv[1]);

    memset(&server_add, 0, sizeof(server_add));
    server_add.sin_addr.s_addr = htonl(INADDR_ANY);
    server_add.sin_family = AF_INET;
    server_add.sin_port = htons(port);

    bind(socketfd, (struct sockaddr*) &server_add, sizeof(server_add));

    listen(socketfd, MAX_CONNECTIONS);

    socket_client1 = accept(socketfd, (struct sockaddr*) &client1_add, &clientLength);
    printf("%d\n", socket_client1);
    socket_client2 = accept(socketfd, (struct sockaddr*) &client2_add, &clientLength);
    printf("%d\n", socket_client2);

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

    winner = analisisDePujas(prize, puja1, puja2);//0 si no gana nadie, 1 si gana p1, 2 si gana p2
    
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