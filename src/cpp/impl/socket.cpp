#include "../dec/socket.h"
#include <iostream>

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

using namespace std;

namespace sockets {
    Socket::Socket() {
        WSADATA wsaData;
        this->ConnectSocket = INVALID_SOCKET;
        struct addrinfo *result = NULL, *ptr = NULL, hints;
        int iResult;

        // Initialize Winsock
        iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
        if (iResult != 0) {
            printf("WSAStartup failed with error: %d\n", iResult);
        }

        ZeroMemory( &hints, sizeof(hints) );
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;

        // Resolve the server address and port
        iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
        if ( iResult != 0 ) {
            printf("getaddrinfo failed with error: %d\n", iResult);
            WSACleanup();
        }

        // Attempt to connect to an address until one succeeds
        for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {

            // Create a SOCKET for connecting to server
            this->ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, 
                ptr->ai_protocol);
            if (this->ConnectSocket == INVALID_SOCKET) {
                printf("socket failed with error: %ld\n", WSAGetLastError());
                WSACleanup();
            }

            // Connect to server.
            iResult = connect(this->ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
            if (iResult == SOCKET_ERROR) {
                closesocket(this->ConnectSocket);
                this->ConnectSocket = INVALID_SOCKET;
                continue;
            }
            break;
        }

        freeaddrinfo(result);

        if (ConnectSocket == INVALID_SOCKET) {
            printf("Unable to connect to server!\n");
            WSACleanup();
        }
    }

    void Socket::sendRequest(char* solicitud) {
        //cout<<solicitud<<endl;
        //cout<< (int)strlen(solicitud)<<endl;
        char* textoNuevo = new char[250];
        strcpy(textoNuevo,solicitud);
        textoNuevo[strlen(solicitud)]='\0';
        int iResult = send(this->ConnectSocket, textoNuevo, (int)strlen(textoNuevo), 0);
        if (iResult == SOCKET_ERROR){
            printf("send failed with error: %d\n", WSAGetLastError());
            closesocket(this->ConnectSocket);
            WSACleanup();
        }
    }

    Socket::~Socket(){
        int iResult = shutdown(this->ConnectSocket, SD_SEND);
        if (iResult == SOCKET_ERROR) {
            printf("shutdown failed with error: %d\n", WSAGetLastError());
            closesocket(this->ConnectSocket);
            WSACleanup();
        }
        // cleanup
        closesocket(ConnectSocket);
        WSACleanup();
        cout<<"Socket cerrado"<<endl;
    }

    void Socket::printBuffer(){
       char *respuesta = new char[DEFAULT_BUFLEN];
       int lenRes = recv(this->ConnectSocket, respuesta, DEFAULT_BUFLEN, 0);
       respuesta[lenRes] = '\0';
       int tam=strlen(respuesta);
       char* textoFinal=new char[tam+1];
       strcpy(textoFinal,respuesta);
       textoFinal[tam]='\0';
       cout<<textoFinal<<endl<<endl;
    }
}