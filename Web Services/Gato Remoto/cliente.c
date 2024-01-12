int main {
    soap_init(&soap);
    serverURL = atoi(argv[1]);

    file = (xsd__string)malloc(NAME_LENGTH);
    file = atoi(argv[2]);

    gatoRemoto_ns__tData Salida;
    gatoRemoto_ns__tFileName fileName;

    fileName.__size = NAME_LENGTH;
    fileName.__ptr = file;

    soap_call_gatoRemoto_ns__execGatoRemoto(&soap, serverURL, NULL, file, &Salida);

}