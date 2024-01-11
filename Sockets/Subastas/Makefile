pablo: cliente servidorPablo

luque: cliente servidorLuque

javi: clienteJavi servidorJavi

clean:
	rm cliente servidor
	clear

cliente: client.c
	gcc -g -Wall client.c -o cliente

servidorPablo: serverPablo.c
	gcc -g -Wall serverPablo.c -lpthread -o servidor
	
servidorLuque: serverLuque.c
	gcc -g -Wall serverLuque.c -lpthread -o servidor
	
clienteJavi: clientJavi.c
	gcc -g -Wall clientJavi.c -o cliente
	
servidorJavi: serverJavi.c
	gcc -g -Wall serverJavi.c -lpthread -o servidor
