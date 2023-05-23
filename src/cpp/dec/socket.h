#ifndef SOCKET_H_
#define SOCKET_H_

#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x501

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>


// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

namespace sockets {
    class Socket {
        private:
            SOCKET ConnectSocket = INVALID_SOCKET;
        public:
            //Constructor que inicializa, crea el socket y se conecta al servidor.
            Socket();

            //Da por finalizada la conexion.
            ~Socket();

            //Envia una solicitud al servidor.
            void sendRequest(char *solicitud);

            //Imprime el buffer de recepcion.
            void printBuffer();
    };
}

#endif