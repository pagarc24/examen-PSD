#include "soapH.h"
#include "wsFileServer.nsmap"
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


#define DEBUG 1

int calculateFileSize (char* fileName);


int main(int argc, char **argv){ 

  int primarySocket, secondarySocket;
  struct soap soap;

  	if (argc < 2) {
    	printf("Usage: %s <port>\n",argv[0]); 
		exit(-1); 
  	}

	// Init environment
  	soap_init(&soap);

	// Bind 	
	

	// Check result of binding		


	// Listen to next connection
	while (1) { 

		// accept
	    

		// Execute invoked operation
	  

		// Clean up!
	  	
	}

  return 0;
}


int calculateFileSize (char* fileName){

	struct stat st;
	int result;

		if (stat(fileName, &st) == -1){
			printf ("[calculateFileSize] Error while executing stat(%s)\n", fileName);
			result = -1;
		}
		else{
			result = st.st_size;
		}

	return(result);
}


int wsFilens__getFileSize (struct soap *soap, wsFilens__tFileName fileName, int *result){

	*result = calculateFileSize (fileName.__ptr);

	if (DEBUG)
		printf ("wsFilens__getFileSize (%s) = %d\n", fileName.__ptr, *result);

	return(SOAP_OK);
}


int wsFilens__createFile (struct soap *soap, wsFilens__tFileName fileName, int *result){

	int fd;

		*result = 0;
			
		// Create file	
		fd = open(fileName.__ptr, O_WRONLY | O_TRUNC | O_CREAT, 0777);

		// Error?
		if (fd < 0){
			printf ("[wsFilens__createFile] Error while creating(%s)\n", fileName.__ptr);
			*result = -1;
		}
		else
			close (fd);

		if (DEBUG)
			printf ("wsFilens__createFile (%s) = %d\n", fileName.__ptr, *result);


	return (SOAP_OK);
}

