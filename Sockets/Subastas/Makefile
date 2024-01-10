luque: cliente servidorLuque

pablo: cliente servidorPablo

clean:
	rm cliente servidor
	clear

cliente: client.c
	gcc -g -Wall client.c -o cliente

servidorPablo: serverPablo.c
	gcc -g -Wall serverPablo.c -lpthread -o servidor
	
servidorLuque: serverLuque.c
	gcc -g -Wall serverLuque.c -lpthread -o servidor
