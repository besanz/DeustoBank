#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dec/cliente.h"
#include "dec/cuenta.h"
#include "dec/transaccion.h"

void pantalla_inicio();
void menu_principal();
void menu_administrador();
void menu_cliente();
int main();

void pantalla_inicio() {
    printf("Bienvenido al DeustoBank\n");
}

void menu_principal() {
    printf("1. Iniciar sesion\n");
    printf("2. Registrarse\n");
    printf("3. Salir\n");
}

void menu_administrador() {
    printf("1. Gestionar clientes\n");
    printf("2. Gestionar cuentas\n");
    printf("3. Gestionar transacciones\n");
    printf("4. Salir\n");
}

void menu_cliente() {
    printf("1. Ver informacion de cuenta\n");
    printf("2. Realizar deposito\n");
    printf("3. Realizar retiro\n");
    printf("4. Realizar transferencia\n");
    printf("5. Ver transacciones\n");
    printf("6. Salir\n");
}

int main() {
    int opcion;
    pantalla_inicio();

    while (1) {
        menu_principal();
        printf("Ingrese la opcion deseada: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1: // Iniciar sesion
                // Logica para iniciar sesion
                break;
            case 2: // Registrarse
                // Logica para registrarse
                break;
            case 3: // Salir
                printf("Gracias por utilizar DeustoBank. Hasta luego!\n");
                exit(0);
            default:
                printf("Opcion no valida. Por favor, intente de nuevo.\n");
        }
    }

    return 0;
}
