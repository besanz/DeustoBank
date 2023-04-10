#include <iostream>
#include "../dec/socket_client.h"

int main() {
    const char* ip_address = "127.0.0.1";
    int port = 8080;

    SOCKET client_socket = iniciar_conexion_cliente(ip_address, port);
    std::cout << "Cliente conectado al servidor: " << ip_address << ":" << port << std::endl;

    // Enviar un mensaje al servidor
    const char* message = "Hola desde el cliente!";
    send(client_socket, message, strlen(message), 0);

    // Recibir respuesta del servidor
    char buffer[1024];
    int recv_size = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
    if (recv_size > 0) {
        buffer[recv_size] = '\0';
        std::cout << "Respuesta del servidor: " << buffer << std::endl;
    } else {
        std::cerr << "Error al recibir datos del servidor" << std::endl;
    }

    // Cerrar la conexión
#ifdef _WIN32
    closesocket(client_socket);
    WSACleanup();
#else
    close(client_socket);
#endif

    return 0;
}
