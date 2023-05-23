#include "../dec/menu.h"



void enviarMenuInicial(SOCKET ClientSocket){
    send(ClientSocket, (char *)"1. Iniciar sesion.\n2. Registrarse.\nq. Salir.\n\n",strlen((char *)"1. Iniciar sesion.\n2. Registrarse.\n3. Salir.\n\n"), 0);
}
void enviarMenuUsuario(SOCKET ClientSocket){
    send(ClientSocket, (char *)"21. Mostrar informacion del cliente\n22. Mostrar informacion de la cuenta\n23. Depositar dinero\n24. Retirar dinero\n25. Realizar transferencia\n26. Mostrar mis transacciones\n27. Imprimir informe financiero\n28. Cerrar mi cuenta.\nq. Salir \n",strlen((char *)"21. Mostrar informacion del cliente\n22. Mostrar informacion de la cuenta\n23. Depositar dinero\n24. Retirar dinero\n25. Realizar transferencia\n26. Mostrar mis transacciones\n27. Imprimir informe financiero\n28. Cerrar mi cuenta.\n29. Salir \n"), 0);
}