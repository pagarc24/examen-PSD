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

void puja(int socketfd){
    char description[STRING_LENGTH];
    int prize, msgLength, pujaUsuario;

    recv(socketfd, &msgLength, sizeof(msgLength), 0);
    recv(socketfd, &description, msgLength, 0);
    recv(socketfd, &prize, sizeof(prize), 0);

    printf("%s -> %d euros\n", description, prize);

    printf("INSERTE SU PUJA\n");
    scanf("%d", &pujaUsuario);
    send(socketfd, &pujaUsuario, sizeof(pujaUsuario), 0);
}

int main(int argc, char *argv[]){
    int socketfd, port, code, finExec = FALSE, msgLength;
    struct sockaddr_in server_add;
    char* serverIP;
    //char* playerName;
    tString playerName;

    serverIP = argv[1];
    port = atoi(argv[2]);

    socketfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    memset(&server_add, 0, sizeof(server_add));
    server_add.sin_addr.s_addr = inet_addr(serverIP);
    server_add.sin_family = AF_INET;
    server_add.sin_port = htons(port);

    connect(socketfd, (struct sockaddr*) &server_add, sizeof(server_add));

    while(!finExec){
        recv(socketfd, &code, sizeof(code), 0);
        
        switch (code){
            case CONNECTION_OK:
                printf("Inserte su nombre: ");
                memset(playerName, 0, STRING_LENGTH);
                fgets(playerName, STRING_LENGTH - 1, stdin);
                playerName[strlen(playerName)-1]=0;

                //msgLength = strlen(playerName)-1;
                
                //send(socketfd, &playerName, msgLength, 0);
                send(socketfd, playerName, strlen(playerName), 0);
                
                puja(socketfd);
                break;
            case BID_WIN:
                printf("\nGANASTE LA PUJA\n");
                finExec = TRUE;
                break;
            case BID_LOSE:
                printf("\nPERDISTE LA PUJA\n");
                finExec = TRUE;
                break;
            default: break;
            }
    }
    
    return 0;
}