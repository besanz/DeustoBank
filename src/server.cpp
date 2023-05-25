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
    Usuario* us;
    Cliente* cli;
    Cliente* destinatario;
    CuentaBancaria* cb;
    Informe* informe;

    vector<string> results;
    results=split(solicitud,';');
    const int opcion= stoi(results[0]);
    const int volver=99;
    const int empezar=0;
    const int inicioSesion=1;
    const int registro=2;
    const int validarUser=11;
    const int registrar=12;
    const int mostrarInformacionCliente=21;
    const int mostrarInformacionCuenta=22;
    const int depositar=23;
    const int realizarDeposito=29;
    const int retirar=24;
    const int realizarRetiro=241;
    const int transferencia=25;
    const int realizarTransferencia=251;
    const int mostrarTransacciones=26;
    const int imprimirInformeFinanciero=27;
    const int cerrarCuenta=28;
    

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
            send(ClientSocket, (char*)"Introduzca los datos con el siguiente formato:\n12;Nombre de usuario;Contrasena;Nombre;Apellido;DNI;Direccion;Telefono;", strlen((char*)"Introduzca los datos con el siguiente formato:\n12;Nombre;Apellido;DNI;Direccion;Telefono;"), 0);
            break;
        case registrar:
            strcpy(us->nombreUsuario, (results[1].c_str()));
            strcpy(us->contrasena, (results[2].c_str()));
            
            strcpy(cli->nombre, (results[3].c_str()));
            strcpy(cli->apellido, (results[4].c_str()));
            strcpy(cli->dni, (results[5].c_str()));
            strcpy(cli->direccion, (results[6].c_str()));
            strcpy(cli->telefono, (results[7].c_str()));

            db_registrar_usuario(us);
            db_registrar_cliente(cli, us);

            enviarMenuUsuario(ClientSocket);
            
            break;
        case validarUser: 
            char user[50];
            strcpy(user, results[1].c_str());
            printf("%s", user);
            int existe;
            existe = db_existe_usuario(user);
            if(existe==1){
                enviarMenuUsuario(ClientSocket);
            }else{
                send(ClientSocket, (char *)"Contraseña o usuario incorrecto. \n Introduzca usuario y contrasenya con el siguiente formato: \n 11;Usuario;Contrasenya",strlen((char *)"Contraseña o usuario incorrecto. \n Introduzca usuario y contrasenya con el siguiente formato: \n 11;Usuario;Contrasenya"), 0);
            }
            
            break;
        case volver:
            enviarMenuUsuario(ClientSocket);
            break;
        case mostrarInformacionCliente:
            char bufferCliente[500];
            int usuarioID;
            usuarioID=db_obtener_usuarioID(results[1].c_str());
            cli=db_buscar_cliente_por_usuarioID(usuarioID);
            // Formatea la información del cliente en el buffer
            sprintf(bufferCliente, "Nombre: %s\nApellido: %s\nDNI: %s\nDireccion: %s\nTelefono: %s\n Para volver 99\n", cli->nombre, cli->apellido, cli->dni, cli->direccion, cli->telefono);

            // Envía el buffer al socket
            send(ClientSocket, bufferCliente, strlen(bufferCliente), 0);
            
            break;
        case mostrarInformacionCuenta:
            char bufferCuenta[500];
            int usID;
            usID=db_obtener_usuarioID(results[1].c_str());
            cli=db_buscar_cliente_por_usuarioID(usID);
            cb=db_buscar_cuenta_por_cliente(cli->clienteID);
            sprintf(bufferCuenta, "Número de cuenta: %s\nSaldo: %.2f\nNombre del cliente: %s\n Para volver 99\n", cb->numeroCuenta, cb->saldo, cb->cliente->nombre);
            send(ClientSocket, bufferCuenta, strlen(bufferCuenta), 0);
            
            break;
        case depositar:
            enviarMenuDepositar(ClientSocket);
            break;
        case realizarDeposito:
            int useID;
            useID=db_obtener_usuarioID(results[1].c_str());
            cli=db_buscar_cliente_por_usuarioID(useID);
            float cantidad;
            cantidad=std::stof(results[2]);
            db_depositar_dinero(cli->clienteID, cantidad);
            send(ClientSocket, (char*)"Deposito realizado correctamente, introduzca 99 para volver.", strlen((char*)"Deposito realizado correctamente, introduzca 99 para volver."),0);
            
            break;

        case imprimirInformeFinanciero:
            char bufferInforme[500];
            int uID;
            uID = db_obtener_usuarioID(results[1].c_str());
            cli = db_buscar_cliente_por_usuarioID(uID);
            informe = generar_informe_financiero(cli->clienteID);

            sprintf(bufferInforme, "Número de cuenta: %s\nNombre del titular: %s\n Saldo final: %.2f\n Numero de depositos: %d\n Total de depositos: %.2f\n Numero de retiros: %d\n Total de retiros: %.2f\n Numero de transferencias enviadas: %d\n Total de transferencias enviadas: %.2f\n Numero de transferencias recibidas: %d\nTotal de transferencias recibidas: %.2f\n Para volver 99\n", informe->numeroCuenta, informe->nombreTitular, informe->saldoFinal, informe->numDepositos, informe->totalDepositos, informe->numRetiros, informe->totalRetiros, informe->numTransferenciasEnviadas, informe->totalTransferenciasEnviadas, informe->numTransferenciasRecibidas, informe->totalTransferenciasRecibidas);
            send(ClientSocket, bufferInforme, strlen(bufferInforme), 0);
            break;
        case cerrarCuenta:
            int userID;
            userID = db_obtener_usuarioID(results[1].c_str());
            cli = db_buscar_cliente_por_usuarioID(userID);
            db_cerrar_cuenta(cli->clienteID);
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

        int tam = strlen(recvbuf);
        char* textoFinal = new char[tam + 1];
        strcpy(textoFinal, recvbuf);
        textoFinal[tam] = '\0';

        if (iResult > 0) {
            // Eliminar caracteres no imprimibles en textoFinal
            for (int i = 0; textoFinal[i] != '\0'; i++) {
                if (!isprint(textoFinal[i])) {
                    for (int j = i; textoFinal[j] != '\0'; j++) {
                        textoFinal[j] = textoFinal[j + 1];
                    }
                    i--;
                }
            }

            cout << "Solicitud recibida: " << textoFinal << endl;
            interpretar(ClientSocket, textoFinal);
            if (iSendResult == SOCKET_ERROR) {
                printf("send failed with error: %d\n", WSAGetLastError());
                closesocket(ClientSocket);
                WSACleanup();
                return 1;
            }
        }
        else if (iResult == 0) {
            printf("Cerrando conexion con el cliente...\n");
        }
        else {
            printf("recv failed with error: %d\n", WSAGetLastError());
            closesocket(ClientSocket);
            WSACleanup();
            cout << iResult << endl;
            return 1;
        }

    }while(iResult > 0);


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