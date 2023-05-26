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
    #include "c/dec/cuenta.h" 
}

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

using namespace std;

void interpretar(SOCKET ClientSocket, char *solicitud){ 
    Usuario* us=nullptr;
    Cliente* cli=nullptr;
    Cliente* destinatario=nullptr;
    CuentaBancaria* cb=nullptr;
    Informe* informe=nullptr;

    vector<string> results=split(solicitud,';');
    const int opcion= stoi(results[0]);
    const int volver=99;
    const int salir=100;
    const int empezar=0;
    const int inicioSesion=1;
    const int registro=2;
    const int validarUser=11;
    const int registrar=12;
    const int mostrarInformacionCliente=21;
    const int mostrarInformacionCliente1=211;
    const int mostrarInformacionCuenta=22;
    const int mostrarInformacionCuenta1=221;
    const int depositar=23;
    const int realizarDeposito=231;
    const int retirar=24;
    const int realizarRetiro=241;
    const int realizarRetiro1=242;
    const int transferencia=25;
    const int realizarTransferencia=251;
    const int imprimirInformeFinanciero=26;
    const int imprimirInformeFinanciero1=261;
    const int cerrarCuenta=27;
    const int cerrarCuenta1=271;

    
    us=new Usuario();
    cli=new Cliente();
    //Aqui empieza el menu
    switch(opcion){
        case empezar:
            //Para responder al cliente: send(ClientSocket, respuesta, strlen(respuesta), 0);
            enviarMenuInicial(ClientSocket);
            break;
        case inicioSesion:
            send(ClientSocket, (char *)"Introduzca usuario y contrasena con el siguiente formato: \n 11;Usuario;Contrasenya;", strlen((char *)"Introduzca usuario y contrasena con el siguiente formato: \n 11;Usuario;Contrasenya;"), 0); 
            results.clear();  
            break; 
        case validarUser: 
            char user[50];
            strcpy(user, results[1].c_str());
            char contra [50];
            strcpy(contra, results[2].c_str());
            int existe;
            existe = db_existe_usuario(user);
            if(existe==1){          
                enviarMenuUsuario(ClientSocket);
            }else{
                send(ClientSocket, (char *)"Contrasenya o usuario incorrecto. \n Introduzca usuario y contrasenya con el siguiente formato: \n 11;Usuario;Contrasenya",strlen((char *)"Contrasenya o usuario incorrecto. \n Introduzca usuario y contrasenya con el siguiente formato: \n 11;Usuario;Contrasenya"), 0);
            }
            results.clear();
            break;
        case registro:
            send(ClientSocket, (char*)"Introduzca los datos con el siguiente formato: 12;nombre de usuario;contrasenya;nombre;apellido;dni;direccion;telefono;", strlen((char*)"Introduzca los datos con el siguiente formato: 12;nombre de usuario;contrasenya;nombre;apellido;dni;direccion;telefono;"),0);
            results.clear();
        case volver:
            enviarMenuUsuario(ClientSocket);
            results.clear();
            break;
        case mostrarInformacionCliente:
            send(ClientSocket, (char*)"Por temas de seguridad introduzca usuario y contrasenya con el siguiente formato: 211;usuario;contrasenya;",strlen((char*)"Por temas de seguridad introduzca usuario y contrasenya con el siguiente formato: 211;usuario;contrasenya;"),0);            
            
            break;
        case mostrarInformacionCliente1:
            char userCliente[50];
            strcpy(userCliente, results[1].c_str());
            char contraCliente[50];
            strcpy(contraCliente, results[2].c_str());
            
            us=db_validar_credenciales(userCliente, contraCliente);
            if(us!=nullptr){          
                char bufferCliente[500];
                int usuarioID;
                usuarioID=db_obtener_usuarioID(results[1].c_str());
            
                cli = db_buscar_cliente_por_usuarioID(usuarioID);
                
                sprintf(bufferCliente, "Nombre: %s\nApellido: %s\nDNI: %s\nDireccion: %s\nTelefono: %s\n Para volver 99\n", cli->nombre, cli->apellido, cli->dni, cli->direccion, cli->telefono);
                send(ClientSocket, bufferCliente, strlen(bufferCliente), 0);
            }else{
                send(ClientSocket, (char *)"Contrasenya o usuario incorrecto. \n Introduzca usuario y contrasenya con el siguiente formato: \n 211;Usuario;Contrasenya",strlen((char *)"Contrasenya o usuario incorrecto. \n Introduzca usuario y contrasenya con el siguiente formato: \n 211;Usuario;Contrasenya"), 0);
            }
            
            results.clear();
            break;
        case mostrarInformacionCuenta:
            send(ClientSocket, (char*)"Por temas de seguridad introduzca usuario y contrasenya con el siguiente formato: 221;usuario;contrasenya;",strlen((char*)"Por temas de seguridad introduzca usuario y contrasenya con el siguiente formato: 221;usuario;contrasenya;"),0);            
            
            results.clear();
            break;
        case mostrarInformacionCuenta1:
            char userCuenta[50];
            strcpy(userCuenta, results[1].c_str());
            char contraCuenta[50];
            strcpy(contraCuenta, results[2].c_str());
            
            us=db_validar_credenciales(userCuenta, contraCuenta);
            if(us!=nullptr){
                char bufferCuenta[500];
                int usID;
                usID=db_obtener_usuarioID(userCuenta);
                cli=db_buscar_cliente_por_usuarioID(usID);
                cb=db_buscar_cuenta_por_cliente(cli->clienteID);
                sprintf(bufferCuenta, "Número de cuenta: %s\nSaldo: %.2f\nNombre del cliente: %s\n Para volver 99\n", cb->numeroCuenta, cb->saldo, cb->cliente->nombre);
                send(ClientSocket, bufferCuenta, strlen(bufferCuenta), 0);

            }else{
                send(ClientSocket, (char *)"Contrasenya o usuario incorrecto. \n Introduzca usuario y contrasenya con el siguiente formato: \n 221;Usuario;Contrasenya",strlen((char *)"Contrasenya o usuario incorrecto. \n Introduzca usuario y contrasenya con el siguiente formato: \n 221;Usuario;Contrasenya"), 0);
            }
            results.clear();
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
        case retirar:
            send(ClientSocket, (char*)"Por temas de seguridad introduzca usuario y contrasenya con el siguiente formato: 241;usuario;contrasenya;cantidad;",strlen((char*)"Por temas de seguridad introduzca usuario y contrasenya con el siguiente formato: 241;usuario;contrasenya;cantidad;"),0);            
            break;
        case realizarRetiro:
            int usuarID;
            usuarID=db_obtener_usuarioID(results[1].c_str());
            printf("%i",usuarID);
            cli=db_buscar_cliente_por_usuarioID(usuarID);
            printf("%i", cli->clienteID);
            float suma;
            suma=std::stof(results[3]);
            db_retirar_dinero(cli->clienteID, suma);
            send(ClientSocket, (char*)"Retiro realizado correctamente, introduzca 99 para volver.", strlen((char*)"Retiro realizado correctamente, introduzca 99 para volver."),0);
            break;
        case transferencia:
            send(ClientSocket, (char*)"Para realizar la transferencia introduzca los datos con este formato: 251;nombre de usuario;destinatario;cantidad;",strlen((char*)"Para realizar la transferencia introduzca los datos con este formato: 251;nombre de usuario;destinatario;cantidad;"),0);
            break;
        case realizarTransferencia:
            int destinatarioID;
            destinatarioID=db_obtener_usuarioID(results[2].c_str());
            destinatario=db_buscar_cliente_por_usuarioID(destinatarioID);

            int usuID;
            usuID=db_obtener_usuarioID(results[1].c_str());
            cli=db_buscar_cliente_por_usuarioID(usuID);
           
            float canti;
            canti=stof(results[3].c_str());
            transferir_dinero(cli->clienteID,destinatario->clienteID,canti);
            send(ClientSocket,(char*)"Transferencia realizada correctamente pulse 99 para volver. ",strlen((char*)"Transferencia realizada correctamente pulse 99 para volver. "),0);
            break;
        case imprimirInformeFinanciero:
            send(ClientSocket, (char*)"Por temas de seguridad introduzca usuario y contrasenya con el siguiente formato: 261;usuario;contrasenya;",strlen((char*)"Por temas de seguridad introduzca usuario y contrasenya con el siguiente formato: 261;usuario;contrasenya;"),0);            
            break;
        case registrar:
            us = new Usuario();
            cli = new Cliente();

            strcpy(us->nombreUsuario, results[1].c_str());
            strcpy(us->contrasena, results[2].c_str());
            
            strcpy(cli->nombre, results[3].c_str());
            strcpy(cli->apellido, results[4].c_str());
            strcpy(cli->dni, results[5].c_str());
            strcpy(cli->direccion, results[6].c_str());
            strcpy(cli->telefono, results[7].c_str());
        
            db_registrar_usuario(us);
            db_registrar_cliente(cli, us);
            crear_cuenta(cli->clienteID);
            enviarMenuUsuario(ClientSocket);
            results.clear();
            break;
        case imprimirInformeFinanciero1:
            char userInforme[50];
            strcpy(userInforme, results[1].c_str());
            char contraInforme[50];
            strcpy(contraInforme, results[2].c_str());
            
            us=db_validar_credenciales(userInforme, contraInforme);
            if(us!=nullptr){
                char bufferInforme[500];
                int uID;
                uID = db_obtener_usuarioID(userInforme);
                cli = db_buscar_cliente_por_usuarioID(uID);
                informe = generar_informe_financiero(cli->clienteID);

                sprintf(bufferInforme, "Número de cuenta: %s\nNombre del titular: %s\n Saldo final: %.2f\n Numero de depositos: %d\n Total de depositos: %.2f\n Numero de retiros: %d\n Total de retiros: %.2f\n Numero de transferencias enviadas: %d\n Total de transferencias enviadas: %.2f\n Numero de transferencias recibidas: %d\nTotal de transferencias recibidas: %.2f\n Para volver 99\n", informe->numeroCuenta, informe->nombreTitular, informe->saldoFinal, informe->numDepositos, informe->totalDepositos, informe->numRetiros, informe->totalRetiros, informe->numTransferenciasEnviadas, informe->totalTransferenciasEnviadas, informe->numTransferenciasRecibidas, informe->totalTransferenciasRecibidas);
                send(ClientSocket, bufferInforme, strlen(bufferInforme), 0);
            }else{
                send(ClientSocket, (char *)"Contrasenya o usuario incorrecto. \n Introduzca usuario y contrasenya con el siguiente formato: \n 261;Usuario;Contrasenya",strlen((char *)"Contrasenya o usuario incorrecto. \n Introduzca usuario y contrasenya con el siguiente formato: \n 261;Usuario;Contrasenya"), 0);
            }
            results.clear();
            break;
        case salir:
            send(ClientSocket, (char*)"Sesión finalizada. Gracias por utilizar DeustoBank.", strlen((char*)"Sesión finalizada. Gracias por utilizar DeustoBank."), 0);
            closesocket(ClientSocket); 
            break;
        case cerrarCuenta:
            send(ClientSocket, (char*)"Para confirmar el cierre de la cuenta introduzca usuario y contrasenya con el siguiente formato: 271;usuario;contrasenya;",strlen((char*)"Para confirmar el cierre de la cuenta introduzca usuario y contrasenya con el siguiente formato: 271;usuario;contrasenya;"),0);            
            
            break;
        case cerrarCuenta1:
            int userID;
            userID = db_obtener_usuarioID(results[1].c_str());
            cli = db_buscar_cliente_por_usuarioID(userID);
            db_cerrar_cuenta(cli->clienteID);
            send(ClientSocket, (char*)"Cuenta cerrada con exito, gracias por utilizar DeustoBank", strlen((char*)"Cuenta cerrada con exito, gracias por utilizar DeustoBank"),0);
            break;
    }
    delete us;
    delete cli;
    delete destinatario;
    delete cb;
    delete informe;
    
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

            // Recibir nuevas solicitudes
            memset(recvbuf, 0, DEFAULT_BUFLEN);
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
        delete[]textoFinal;
        
    } while(iResult > 0);



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