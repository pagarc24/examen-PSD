#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#define MAX_CONNECTIONS 10

/*
    SERVIDOR
*/

int main(int argc, char *argv[]){
    int socketfd, port, client_socketfd, clientLength;
    struct sockaddr_in client_add;
    struct sockaddr_in server_add;

    socketfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    port = atoi(argv[1]);

    memset(&server_add, 0, sizeof(server_add));
    server_add.sin_addr.s_addr = htonl(INADDR_ANY);
    server_add.sin_family = AF_INET;
    server_add.sin_port = htons(port);

    bind(socketfd, (struct sockaddr*) &server_add, sizeof(server_add));

    listen(socket, MAX_CONNECTIONS);

    client_socketfd = accept(socketfd, (struct sockaddr*) &client_add, &clientLength);

    //PROGRAMA
    
    return 0;
}


/*
    CLIENTE
*/
int main(int argc, char *argv[]){
    int socketfd, port;
    struct sockaddr_in server_add;
    char* serverIP;

    socketfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    serverIP = argv[1];
    port = atoi(argv[2]);

    memset(&server_add, 0, sizeof(server_add));
    server_add.sin_addr.s_addr = inet_addr(serverIP);
    server_add.sin_family = AF_INET;
    server_add.sin_port = htons(port);

    connect(socketfd, (struct sockaddr*) &server_add, sizeof(server_add));

    //PROGRAMA
    
    return 0;
}
