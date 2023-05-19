#include <iostream>
#include <cstring>
#include <cerrno>
#include "cpp/dec/socket_server.h"
#include "cpp/dec/socket_client.h"
#include "cpp/dec/socket_handler.h"

#ifdef _WIN32
#include <winsock2.h>
#else
#include <unistd.h>
#endif

void handle_client(SOCKET client_socket) {
    char buffer[1024];
    int recv_size;

    while ((recv_size = recv(client_socket, buffer, sizeof(buffer) - 1, 0)) > 0) {
        buffer[recv_size] = '\0';
        std::cout << "Mensaje recibido: " << buffer << std::endl;

        // Enviar respuesta al cliente
        const char *response = "Mensaje recibido por el servidor.";
        send(client_socket, response, strlen(response), 0);
    }
    
    std::cout << "Cliente desconectado." << std::endl;
    closesocket(client_socket);
    
}

int main() {
    const char *ip_address = "127.0.0.1";
    int port = 8080;

    SOCKET server_socket = iniciar_servidor(port);

    std::cout << "Servidor iniciado en puerto " << port << std::endl;

    while (true) {
        struct sockaddr_in client_address;
        int client_address_len = sizeof(client_address);

        SOCKET client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_address_len);
        if (client_socket < 0) {
            std::cerr << "Error al aceptar la conexión del cliente: " << strerror(errno) << std::endl;
            continue;
        }

        std::cout << "Cliente conectado: " << inet_ntoa(client_address.sin_addr) << ":" << ntohs(client_address.sin_port) << std::endl;

        handle_client(client_socket);
    }

    closesocket(server_socket);

    return 0;
}
