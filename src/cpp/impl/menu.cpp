#include "../dec/menu.h"
extern "C"{
    #include "../../c/dec/db.h"
    #include "../../c/struct.h"
    #include "../../../lib/sqlite3.h" 
}


void enviarMenuInicial(SOCKET ClientSocket){
    send(ClientSocket, (char *)"1. Iniciar sesion.\n2. Registrarse.\n100. Salir.\n\n",strlen((char *)"1. Iniciar sesion.\n2. Registrarse.\n100. Salir.\n\n"), 0);
}
void enviarMenuUsuario(SOCKET ClientSocket){
    send(ClientSocket, (char *)"21. Mostrar informacion del cliente\n22. Mostrar informacion de la cuenta\n23. Depositar dinero\n24. Retirar dinero\n25. Realizar transferencia\n26. Imprimir informe financiero\n27. Cerrar mi cuenta.\n100. Salir \n",strlen((char *)"21. Mostrar informacion del cliente\n22. Mostrar informacion de la cuenta\n23. Depositar dinero\n24. Retirar dinero\n25. Realizar transferencia\n26. Imprimir informe financiero\n27. Cerrar mi cuenta.\n100. Salir \n"), 0);
}
void enviarMenuDepositar(SOCKET ClientSocket){
    send(ClientSocket, (char *)"Ingrese la cantidad que quiere depositar y a quien con el siguiente formato: 231;nombre de usuario;cantidad;\n",strlen((char *)"Ingrese la cantidad que quiere depositar y a quien con el siguiente formato: 231;nombre de usuario;cantidad;\n"), 0);
}
void enviarMenuRetirar(SOCKET ClientSocket){
    send(ClientSocket, (char *)"241.Ingrese la cantidad que quiera retirar:\n",strlen((char *)"241.Ingrese la cantidad que quiera retirar\n"), 0);
}
#include <sstream>



