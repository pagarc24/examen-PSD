all: cliente servidor

clean:
	rm cliente servidor
	clear

cliente: client.c
	gcc -g -Wall client.c -o cliente

servidor: server.c
	gcc -g -Wall server.c -o servidor