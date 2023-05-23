#ifndef _MOSTRAR_H_
#define _MOSTRAR_H_

#undef UNICODE
#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x501


#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>



// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"
//Declaraciones de funciones dirigidas a visualizar informacion en pantalla.

void enviarMenuInicial(SOCKET ClientSocket);
void enviarMenuUsuario(SOCKET ClientSocket);

#endif