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

/* Definiciones dadas en el enunciado */
#define TRUE 1
#define FALSE 0
#define STRING_LENGTH 128
typedef char tString[STRING_LENGTH];
#define MAX_ROOMS 2
#define MAX_CONNECTIONS 4
#define SERVER_FULL 1000
#define CONNECTION_OK 2000
#define BID_WIN 3000
#define BID_LOSE 4000

/*
    SERVIDOR @aluque
*/
#define DESC "Pito orbis terrarum"
#define COST 100

typedef struct
{
  tString desc;
  int cost;
  int puja1;
  tString p1;
  int socket_client1;
  int puja2;
  tString p2;
  int socket_client2
} tSala;

typedef struct threadArgs
{
  int client1_socketfd;
  int client2_socketfd;
} tThreadArgs;

int analisisDePujas(int price, int bid1, int bid2);
void *threadTask(void *args);

int main(int argc, char *argv[])
{
  int socketfd, port, clientLength, msgLength;
  struct sockaddr_in client1_add, client2_add, decline_add;
  struct sockaddr_in server_add;
  tThreadArgs threadArgs[MAX_ROOMS];
  pthread_t threadID[MAX_ROOMS];

  socketfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

  port = atoi(argv[1]);

  memset(&server_add, 0, sizeof(server_add));
  server_add.sin_addr.s_addr = htonl(INADDR_ANY);
  server_add.sin_family = AF_INET;
  server_add.sin_port = htons(port);

  bind(socketfd, (struct sockaddr *)&server_add, sizeof(server_add));

  listen(socketfd, MAX_CONNECTIONS);

  unsigned short n = 0;
  while (n < MAX_ROOMS)
  {
    threadArgs[n].client1_socketfd = accept(socketfd, (struct sockaddr *)&client1_add, &clientLength);
    threadArgs[n].client2_socketfd = accept(socketfd, (struct sockaddr *)&client2_add, &clientLength);
    printf("Socket client1: %d\n", threadArgs[n].client1_socketfd);
    printf("Socket client2: %d\n", threadArgs[n].client2_socketfd);
    pthread_create(&threadID[n], NULL, threadTask, threadArgs);
  }

  int decline_socket = accept(socketfd, (struct sockaddr*)&decline_add, &clientLength);
  send(decline_socket, SERVER_FULL, sizeof(SERVER_FULL), 0);


  for (int i = 0; i < n; ++i)
  {
    pthread_join(threadID[i], NULL);
    exit(0);
  }

  return 0;
}

void *threadTask(void *args)
{
  tSala sala;
  int winner, code;
  tString description = "Batidora con la cara de Messi";
  int prize = 100;
  tThreadArgs *threadArgs;
  int msgLength;

  // get thread args from args
  threadArgs = (tThreadArgs *)args;
  sala.socket_client1 = threadArgs->client1_socketfd;
  sala.socket_client2 = threadArgs->client2_socketfd;

  code = CONNECTION_OK;
  send(sala.socket_client1, &code, sizeof(code), 0);
  send(sala.socket_client2, &code, sizeof(code), 0);

  // Recv nombres
  memset(sala.p1, 0, STRING_LENGTH);
  recv(sala.socket_client1, sala.p1, STRING_LENGTH - 1, 0);
  printf("Jug1: %s\n", sala.p1);

  memset(sala.p2, 0, STRING_LENGTH);
  recv(sala.socket_client2, sala.p2, STRING_LENGTH - 1, 0);
  printf("Jug2: %s\n", sala.p2);

  // msgLength = strlen(description)-1;
  msgLength = strlen(description);

  send(sala.socket_client1, &msgLength, sizeof(msgLength), 0);
  send(sala.socket_client1, &description, msgLength, 0);
  send(sala.socket_client1, &prize, sizeof(prize), 0);

  send(sala.socket_client2, &msgLength, sizeof(msgLength), 0);
  send(sala.socket_client2, &description, msgLength, 0);
  send(sala.socket_client2, &prize, sizeof(prize), 0);

  printf("\nRESULTADOS DE LA PUJA POR %s(%d)\n\n", description, prize);
  recv(sala.socket_client1, &sala.puja1, sizeof(sala.puja1), 0);
  printf("%s -> %d\n", sala.p1, sala.puja1);
  recv(sala.socket_client2, &sala.puja2, sizeof(sala.puja2), 0);
  printf("%s -> %d\n", sala.p2, sala.puja2);

  winner = analisisDePujas(prize, sala.puja1, sala.puja2); // 0 si no gana nadie, 1 si gana p1, 2 si gana p2

  if (winner == 0)
  {
    code = BID_LOSE;
    send(sala.socket_client1, &code, sizeof(code), 0);
    send(sala.socket_client2, &code, sizeof(code), 0);
  }
  else if (winner == 1)
  {
    code = BID_WIN;
    send(sala.socket_client1, &code, sizeof(code), 0);
    code = BID_LOSE;
    send(sala.socket_client2, &code, sizeof(code), 0);
  }
  else if (winner == 2)
  {
    code = BID_LOSE;
    send(sala.socket_client1, &code, sizeof(code), 0);
    code = BID_WIN;
    send(sala.socket_client2, &code, sizeof(code), 0);
  }

  // termninate connections and thread
  close(sala.socket_client1);
  close(sala.socket_client2);
  pthread_exit(NULL);
}
/* ESTA VERSION PASA DE MI Y EJECUTA EL SERVER COMO SI FUESE UN PROGRAMA NORMAL
int main(int argc, char *argv[])
{
  int socketfd, port;
  struct sockaddr_in server_addr;
  // tSala casa_subastas[MAX_ROOMS];
  // tThreadArgs threadArgs[MAX_ROOMS];
  // pthread_t threadID[MAX_ROOMS];
  tSala sala1;
  int code;
  int winner;
  struct sockaddr_in client_addr;
  unsigned int clientAddrLength = sizeof(client_addr);

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

  listen(socket, MAX_CONNECTIONS);

  // Cuando se conecte un cliente

  sala1.client1_socketfd = accept(socketfd, (struct sockaddr *)&client_addr, &clientAddrLength);
  printf("Cliente 1 : [%d]\n", sala1.client1_socketfd);
  sala1.client2_socketfd = accept(socketfd, (struct sockaddr *)&client_addr, &clientAddrLength);
  printf("Cliente 2 : [%d]\n", sala1.client2_socketfd);

  // Send CONNECTION_OK to both clients
  send(sala1.client1_socketfd, CONNECTION_OK, sizeof(CONNECTION_OK), 0);
  send(sala1.client2_socketfd, CONNECTION_OK, sizeof(CONNECTION_OK), 0);

  // Recieve client1 name
  memset(sala1.client1_name, 0, STRING_LENGTH);
  recv(socketfd, sala1.client1_name, STRING_LENGTH - 1, 0);
  printf("Cliente1: %s\n", sala1.client1_name);

  // Recieve client2 name
  memset(sala1.client2_name, 0, STRING_LENGTH);
  recv(socketfd, sala1.client2_name, STRING_LENGTH - 1, 0);
  printf("Cliente1: %s\n", sala1.client1_name);

  // Send description to both clients
  send(sala1.client1_socketfd, DESC, sizeof(DESC), 0);
  send(sala1.client2_socketfd, DESC, sizeof(DESC), 0);
  // Send cost to both clients
  send(sala1.client1_socketfd, COST, sizeof(COST), 0);
  send(sala1.client2_socketfd, COST, sizeof(COST), 0);

  // Recieve client1 bid
  recv(sala1.client1_socketfd, sala1.client1_bid, sizeof(sala1.client1_bid), 0);
  // Recieve client2 bid
  recv(sala1.client2_socketfd, sala1.client2_bid, sizeof(sala1.client2_bid), 0);

  printf("\nRESULTADOS DE LA PUJA POR %s(%d)\n\n", DESC, COST);
  recv(sala1.client1_socketfd, &sala1.client1_bid, sizeof(sala1.client1_bid), 0);
  printf("%s -> %d\n", sala1.client1_name, sala1.client1_bid);
  recv(sala1.client2_socketfd, &sala1.client2_bid, sizeof(sala1.client2_bid), 0);
  printf("%s -> %d\n", sala1.client2_name, sala1.client2_bid);

  winner = analisisDePujas(COST, sala1.client1_bid, sala1.client2_bid); // 0 si no gana nadie, 1 si gana p1, 2 si gana p2

  if (winner == 0)
  {
    code = BID_LOSE;
    send(sala1.client1_socketfd, &code, sizeof(code), 0);
    send(sala1.client2_socketfd, &code, sizeof(code), 0);
  }
  else if (winner == 1)
  {
    code = BID_WIN;
    send(sala1.client1_socketfd, &code, sizeof(code), 0);
    code = BID_LOSE;
    send(sala1.client2_socketfd, &code, sizeof(code), 0);
  }
  else if (winner == 2)
  {
    code = BID_LOSE;
    send(sala1.client1_socketfd, &code, sizeof(code), 0);
    code = BID_WIN;
    send(sala1.client2_socketfd, &code, sizeof(code), 0);
  }

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


  return 0;
}
*/
int analisisDePujas(int price, int bid1, int bid2)
{
  int winner;

  if (bid1 > price || bid2 > price)
  {
    if (bid1 > bid2)
      winner = 1;
    else if (bid2 > bid1)
      winner = 2;
    else
      winner = 0;
  }
  else
    winner = 0;

  return winner;
}