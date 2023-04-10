#pragma once

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include "../../c/dec/cliente.h"
#include "../../c/dec/cuenta.h"
#include "../../c/dec/transaccion.h"
#include "../../c/dec/db.h"
#include "../../c/dec/usuario.h"

// Declaraciones de funciones para interactuar con los sockets en C++
Cliente *recibir_cliente(SOCKET client_socket);
void enviar_cliente(SOCKET client_socket, Cliente *cliente);

CuentaBancaria *recibir_cuenta(SOCKET client_socket);
void enviar_cuenta(SOCKET client_socket, CuentaBancaria *cuenta);

Transaccion *recibir_transaccion(SOCKET client_socket);
void enviar_transaccion(SOCKET client_socket, Transaccion *transaccion);

Usuario *recibir_usuario(SOCKET client_socket);
void enviar_usuario(SOCKET client_socket, Usuario *usuario);

#ifdef __cplusplus
}
#endif
