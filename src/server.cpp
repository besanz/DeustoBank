#undef UNICODE

#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x501

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include "cpp/dec/split.h"
#include "cpp/dec/menu.h"

extern "C"{
    #include "c/dec/db.h"
    #include "c/struct.h"
    #include "../lib/sqlite3.h" 
}

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

using namespace std;

void interpretar(SOCKET ClientSocket, char *solicitud){
    Usuario *usuario;
    Cliente *cliente;
    vector<string> results;
    results=split(solicitud,';');
    for(int i =0; i<results.size()-1;i++){
        cout<<results[i]<<endl;
    }
    

    const int opcion= stoi(results[0]);
    const int empezar=0;
    const int inicioSesion=1;
    const int registro=2;
    const int validarUser=11;
    
    //Aqui empieza el menu
    switch(opcion){
        case empezar:
            //Para responder al cliente: send(ClientSocket, respuesta, strlen(respuesta), 0);
            enviarMenuInicial(ClientSocket);
            break;
        case inicioSesion:
            send(ClientSocket, (char *)"Introduzca usuario y contrasena con el siguiente formato: \n 11;Usuario;Contrasenya;", strlen((char *)"Introduzca usuario y contrasena con el siguiente formato: \n 11;Usuario;Contrasenya;"), 0);   
            break; 
            
        case registro:

            break;
                
        case validarUser: 
            char* user = new char[results[1].length() + 1];
            strcpy(user, (results[1].c_str()));

            char* contra = new char[results[2].length() + 1];
            strcpy(contra, (results[2].c_str()));

            Usuario* usu= db_validar_credenciales(user,contra);
            
            
            if(usu!=nullptr){
                //enviarMenuUsuario(ClientSocket);
                cout<<"Coge bien el usuario"<<endl;
            }
            else {
                //send(ClientSocket, (char *)"Contraseña o usuario incorrecto. \n Introduzca usuario y contrasenya con el siguiente formato: \n 11;Usuario;Contrasenya",strlen((char *)"Contraseña o usuario incorrecto. \n Introduzca usuario y contrasenya con el siguiente formato: \n 11;Usuario;Contrasenya"), 0);
                cout<<"No coge bien el usuario"<<endl;
            } 
        break;
    }
    
    
   

}


int main(int argc, char* argv[]) 
{
    WSADATA wsaData;
    int iResult;

    SOCKET ListenSocket = INVALID_SOCKET;
    SOCKET ClientSocket = INVALID_SOCKET;

    struct addrinfo *result = NULL;
    struct addrinfo hints;

    int iSendResult;
    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;
    
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if ( iResult != 0 ) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Create a SOCKET for connecting to server
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    // Setup the TCP listening socket
    iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    freeaddrinfo(result);

    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    cout<<"Servidor preparado. A la espera de solicitudes."<<endl<<endl;

    // Accept a client socket
    ClientSocket = accept(ListenSocket, NULL, NULL);
    if (ClientSocket == INVALID_SOCKET) {
        printf("accept failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    cout<<"Conexion con servidor establecida"<<endl<<endl;

    // No longer need server socket
    closesocket(ListenSocket);

    // Receive until the peer shuts down the connection
    do {

        iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);

        int tam=strlen(recvbuf);
        char* textoFinal=new char[tam+1];
        strcpy(textoFinal,recvbuf);
        textoFinal[tam]='\0';
       
        if (iResult > 0) {
            cout<<"Solicitud recibida: "<<textoFinal<<endl;
            interpretar(ClientSocket, textoFinal);
            if (iSendResult == SOCKET_ERROR) {
                printf("send failed with error: %d\n", WSAGetLastError());
                closesocket(ClientSocket);
                WSACleanup();
                return 1;
            }
        }
        else if (iResult == 0){
            printf("Cerrando conexion con el cliente...\n");
        }
        else  {
            printf("recv failed with error: %d\n", WSAGetLastError());
            closesocket(ClientSocket);
            WSACleanup();
            cout<<iResult<<endl;
            return 1;
        }

    } while (iResult > 0);

    // shutdown the connection since we're done
    iResult = shutdown(ClientSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(ClientSocket);
        WSACleanup();
        return 1;
    }

    // cleanup
    closesocket(ClientSocket);
    WSACleanup();

    cout<<"Conexion terminada."<<endl;

    return 0;
}