#include "../dec/menu.h"
extern "C"{
    #include "c/dec/db.h"
    #include "c/struct.h"
    #include "../lib/sqlite3.h" 
}


void enviarMenuInicial(SOCKET ClientSocket){
    send(ClientSocket, (char *)"1. Iniciar sesion.\n2. Registrarse.\nq. Salir.\n\n",strlen((char *)"1. Iniciar sesion.\n2. Registrarse.\n3. Salir.\n\n"), 0);
}
void enviarMenuUsuario(SOCKET ClientSocket){
    send(ClientSocket, (char *)"21. Mostrar informacion del cliente\n22. Mostrar informacion de la cuenta\n23. Depositar dinero\n24. Retirar dinero\n25. Realizar transferencia\n26. Mostrar mis transacciones\n27. Imprimir informe financiero\n28. Cerrar mi cuenta.\nq. Salir \n",strlen((char *)"21. Mostrar informacion del cliente\n22. Mostrar informacion de la cuenta\n23. Depositar dinero\n24. Retirar dinero\n25. Realizar transferencia\n26. Mostrar mis transacciones\n27. Imprimir informe financiero\n28. Cerrar mi cuenta.\n29. Salir \n"), 0);
}
void enviarMenuDepositar(SOCKET ClientSocket){
    send(ClientSocket, (char *)"333.Ingrese la cantidad que quiere depositar:\n",strlen((char *)"333.Ingrese la cantidad que quiere depositar\n"), 0);
}
void enviarMenuRetirar(SOCKET ClientSocket){
    send(ClientSocket, (char *)"444.Ingrese la cantidad que quiera retirar:\n",strlen((char *)"333.Ingrese la cantidad que quiera depositar\n"), 0);
}
#include <sstream>

void mostrarCliente(SOCKET ClientSocket, Usuario* usuario) {
    
    Cliente* cliente = db_buscar_cliente_por_usuarioID(usuario->usuarioID);

    // Verificar si se encontró un cliente válido
    if (cliente != NULL) {
        // Construir un mensaje para enviar al cliente con la información del cliente.
        std::stringstream mensaje;
        mensaje << "Información del cliente:\n";
        mensaje << "Nombre: " << cliente->nombre << "\n";
        mensaje << "Apellido: " << cliente->apellido << "\n";
        mensaje << "DNI: " << cliente->dni << "\n";
        mensaje << "Dirección: " << cliente->direccion << "\n";
        mensaje << "Teléfono: " << cliente->telefono << "\n";

        // Envía el mensaje al cliente.
        std::string mensajeStr = mensaje.str();
        send(ClientSocket, mensajeStr.c_str(), mensajeStr.length(), 0);
    } else {
        // Si no se encuentra el cliente, se puede enviar un mensaje indicando la situación.
        send(ClientSocket, "No se encontró información del cliente.", strlen("No se encontró información del cliente."), 0);
    }
}

