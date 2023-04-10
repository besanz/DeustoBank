#define _WINSOCK_DEPRECATED_NO_WARNINGS
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif
#include <winsock2.h>
#include <ws2tcpip.h>
#include "../dec/socket_client.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#define close(s) closesocket(s)
#endif

SOCKET iniciar_conexion_cliente(const char *ip_address, int puerto) {
    SOCKET client_socket;
    struct sockaddr_in server_address;

#ifdef _WIN32
    WSADATA wsa_data;
    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
        perror("Error iniciando Winsock");
        exit(EXIT_FAILURE);
    }
#endif

    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        perror("Error creando socket");
        exit(EXIT_FAILURE);
    }

    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(puerto);
    server_address.sin_addr.s_addr = inet_addr(ip_address);

    if (server_address.sin_addr.s_addr == INADDR_NONE) {
        perror("Error en inet_addr");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("Error en connect");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    return client_socket;
}
