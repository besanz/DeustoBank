#include "../dec/socket_handler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#define close(s) closesocket(s)
#endif

Cliente *recibir_cliente(SOCKET client_socket) {
    Cliente *cliente = (Cliente *)malloc(sizeof(Cliente));
    int received = recv(client_socket, (char *)cliente, sizeof(Cliente), 0);
    if (received <= 0) {
        perror("Error recibiendo cliente");
        return NULL;
    }
    return cliente;
}

void enviar_cliente(SOCKET client_socket, Cliente *cliente) {
    if (send(client_socket, (const char *)cliente, sizeof(Cliente), 0) < 0) {
        perror("Error enviando cliente");
    }
}

CuentaBancaria *recibir_cuenta(SOCKET client_socket) {
    CuentaBancaria *cuenta = (CuentaBancaria *)malloc(sizeof(CuentaBancaria));
    int received = recv(client_socket, (char *)cuenta, sizeof(CuentaBancaria), 0);
    if (received <= 0) {
        perror("Error recibiendo cuenta bancaria");
        return NULL;
    }
    return cuenta;
}

void enviar_cuenta(SOCKET client_socket, CuentaBancaria *cuenta) {
    if (send(client_socket, (const char *)cuenta, sizeof(CuentaBancaria), 0) < 0) {
        perror("Error enviando cuenta bancaria");
    }
}

Transaccion *recibir_transaccion(SOCKET client_socket) {
    Transaccion *transaccion = (Transaccion *)malloc(sizeof(Transaccion));
    int received = recv(client_socket, (char *)transaccion, sizeof(Transaccion), 0);
    if (received <= 0) {
        perror("Error recibiendo transacción");
        return NULL;
    }
    return transaccion;
}

void enviar_transaccion(SOCKET client_socket, Transaccion *transaccion) {
    if (send(client_socket, (const char *)transaccion, sizeof(Transaccion), 0) < 0) {
        perror("Error enviando transacción");
    }
}

Usuario *recibir_usuario(SOCKET client_socket) {
    Usuario *usuario = (Usuario *)malloc(sizeof(Usuario));
    int received = recv(client_socket, (char *)usuario, sizeof(Usuario), 0);
    if (received <= 0) {
        perror("Error recibiendo usuario");
        return NULL;
    }
    return usuario;
}

void enviar_usuario(SOCKET client_socket, Usuario *usuario) {
    if (send(client_socket, (const char *)usuario, sizeof(Usuario), 0) < 0) {
        perror("Error enviando usuario");
    }
}
