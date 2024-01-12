//gsoap gatoRemoto_ns service name: gatoRemoto
//gsoap gatoRemoto_ns service style: rpc
//gsoap gatoRemoto_ns service encoding: encoded
//gsoap gatoRemoto_ns service namespace: urn:remoteCat
/** Length of data buffer */
#define MAX_DATA_SIZE 1024
/** Length for tString */
#define NAME_LENGTH 128
/** Typedef for the file name */
typedef char *xsd__string;
/** Struct that contains a file name */
typedef struct tFileName{
int __size;
xsd__string __ptr;
}gatoRemoto_ns__tFileName;
/** Binary data */
typedef struct tData{
int __size;
unsigned char *__ptr;
}gatoRemoto_ns__tData;
int gatoRemoto_ns__execGatoRemoto (gatoRemoto_ns__tFileName fileName,
gatoRemoto_ns__tData *data);

//---------------------------------------------------------------------------------------------------

int gatoRemoto_ns__execGatoRemoto(gatoRemoto_ns__tFileName fileName, gatoRemoto_ns__tData *data){


    int o = open(fileName);
    data.__size = calculateFileSize (fileName);

    read(o, data.__ptr);                                                    //leemos todo el fiechero

    return SOAP_OK;
}

int main(){
    //Inicializamos el entorno soap y configuramos los timeouts
    soap_init(&soap);
    port = atoi(argv[1]);

    m = soap_bind(&soap, NULL, port, 100);
    while(1){
		soap_accept(&soap);

		soap_serve((struct soap*)soap);
        soap_destroy((struct soap*)soap);
        soap_end(&soap);
	    soap_done(&soap);

		tsoap = soap_copy(&soap);
		
	}

}

//apartado c (DATOS NO FIABLES)
    //varios xml    -> protocolo estardar para selializar los datos
    //un wsld
    //los stubs -> es un representante local del participante remoto
