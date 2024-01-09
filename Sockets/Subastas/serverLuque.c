#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

/* Definiciones dadas en el enunciado */
#define TRUE 1
#define FALSE 0
#define STRING_LENGTH 128
typedef char tString[STRING_LENGTH];
#define MAX_ROOMS 2
#define SERVER_FULL 1000
#define CONNECTION_OK 2000
#define BID_WIN 3000
#define BID_LOSE 4000

/*
    SERVIDOR @aluque
*/
#define desc "Pito orbis terrarum"
#define cost 100

typedef struct
{
  // tString desc;
  // int cost;
  int client1_bid;
  tString client1_name;
  int client1_socketfd;
  int client2_bid;
  tString client2_name;
  int client2_socketfd;
} tSala;

/*
typedef struct threadArgs
{
  int client1_socketfd;
  int client2_socketfd;
} tThreadArgs;
*/

int main(int argc, char *argv[])
{
  int socketfd, port;
  struct sockaddr_in server_addr;
  // tSala casa_subastas[MAX_ROOMS];
  // tThreadArgs threadArgs[MAX_ROOMS];
  // pthread_t threadID[MAX_ROOMS];
  tSala sala1;
  int clients;

  socketfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

  if (argc < 2)
  {
    fprintf(stderr, "ERROR wrong number of arguments\n");
    fprintf(stderr, "Usage:\n$>%s port\n", argv[0]);
    exit(1);
  }

  port = atoi(argv[1]);

  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port);

  bind(socketfd, (struct sockaddr *)&server_addr, sizeof(server_addr));

  listen(socket, MAX_ROOMS);

  // Cuando se conecte un cliente
  struct sockaddr_in client_addr;
  unsigned int clientAddrLength = sizeof(client_addr);
  clients = 0;
  printf("PUTOS CLIENTES : %d\n", clients);
  while(clients < 2)
  {
    if(clients == 0){
      sala1.client1_socketfd = accept(socket, (struct sockaddr *)&client_addr, &clientAddrLength);
      ++clients;
    }
    else if(clients == 1){
      sala1.client2_socketfd = accept(socket, (struct sockaddr *)&client_addr, &clientAddrLength);
      ++clients;
    }
    else{
      int declineSocket = accept(socket, (struct sockaddr *)&client_addr, &clientAddrLength);
      send(declineSocket, SERVER_FULL, sizeof(SERVER_FULL), 0);
    }
  }
  // Send CONNECTION_OK to both clients
  send(sala1.client1_socketfd, CONNECTION_OK, sizeof(CONNECTION_OK), 0);
  send(sala1.client2_socketfd, CONNECTION_OK, sizeof(CONNECTION_OK), 0);
  
  // Recieve client1 name
  memset(sala1.client1_name, 0, STRING_LENGTH);
  recv(socketfd, sala1.client1_name, STRING_LENGTH - 1, 0);
  
  // Recieve client2 name
  memset(sala1.client2_name, 0, STRING_LENGTH);
  recv(socketfd, sala1.client2_name, STRING_LENGTH - 1, 0);

  // Send description to both clients
  send(sala1.client1_socketfd, desc, sizeof(desc), 0);
  send(sala1.client2_socketfd, desc, sizeof(desc), 0);
  // Send cost to both clients
  send(sala1.client1_socketfd, cost, sizeof(cost), 0);
  send(sala1.client2_socketfd, cost, sizeof(cost), 0);

  // Recieve client1 bid
  recv(sala1.client1_socketfd, sala1.client1_bid, sizeof(sala1.client1_bid), 0);
  // Recieve client2 bid
  recv(sala1.client2_socketfd, sala1.client2_bid, sizeof(sala1.client2_bid), 0);

  // print both bids and who made it
  printf("%s: %d\n", sala1.client1_name, sala1.client1_bid);
  printf("%s: %d\n", sala1.client2_name, sala1.client2_bid);

  /*
  unsigned int n = 0;
  for (int i = 0; i < MAX_ROOMS; i++){

      threadArgs[n].client1_socketfd = accept(socketfd, (struct sockaddr*) &client_add, &clientLength);
      threadArgs[n].client2_socketfd = accept(socketfd, (struct sockaddr*) &client_add, &clientLength);
      pthread_create(&threadID[n], NULL, threadFunc, &threadArgs[n]);
      n++;

  }
  for (int i = 0; i < n; i++) {
      pthread_join(threadID[i], NULL);
  }
  */

  return 0;
}