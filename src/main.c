#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cliente.h" // Módulo de gestión de clientes
#include "cuenta.h" // Módulo de gestión de cuentas bancarias
#include "transaccion.h" // Módulo de gestión de operaciones bancarias

// Función para imprimir la pantalla de inicio
void pantalla_inicio() {
    printf("  ___   __________  ___      ___   _____   _____   ___   _   _   _____   \n");
    printf(" / _ \\ |___  / ___||_ _|    / _ \\ | ____| |_   _| |_ _| | \\ | | |_   _|  \n");
    printf("| | | |   / /\\___ \\ | |    | | | ||  _|     | |    | |  |  \\| |   | |    \n");
    printf("| |_| |  / /  ___) || |    | |_| || |___    | |    | |  | |\\  |   | |    \n");
    printf(" \\___/  /_/  |____/|___|    \\___/ |_____|   |_|   |___| |_| \\_|   |_|    \n");
    printf("                         Su banco virtual preferido.                       \n");
    printf("\n");
}

// Función para imprimir el menú principal
void menu_principal() {
    printf("\n--Menú principal--\n");
    printf("1. Iniciar sesión\n");
    printf("2. Crear cuenta bancaria\n");
    printf("3. Mostrar información de una cuenta bancaria\n");
    printf("4. Depositar dinero en una cuenta bancaria\n");
    printf("5. Retirar dinero de una cuenta bancaria\n");
    printf("6. Transferir dinero de una cuenta bancaria a otra\n");
    printf("7. Cerrar cuenta bancaria\n");
    printf("0. Salir\n");
    printf("Por favor, seleccione una opción: ");
}

int main() {
// Variable para almacenar la opción seleccionada por el usuario
int opcion;

// Imprimir la pantalla de inicio
pantalla_inicio();

// Bucle principal del programa
do {
    // Imprimir el menú principal
    menu_principal();

    // Leer la opción seleccionada por el usuario
    scanf("%d", &opcion);

    switch(opcion) {
        case 1:
            // Iniciar sesión
            iniciar_sesion();
            break;
        case 2:
            // Crear cuenta bancaria
            crear_cuenta();
            break;
        case 3:
            // Mostrar información de una cuenta bancaria
            mostrar_informacion_cuenta();
            break;
        case 4:
            // Depositar dinero en una cuenta bancaria
            depositar_dinero();
            break;
        case 5:
            // Retirar dinero de una cuenta bancaria
            retirar_dinero();
            break;
        case 6:
            // Transferir dinero de una cuenta bancaria a otra
            transferir_dinero();
            break;
        case 7:
            // Cerrar cuenta bancaria
            cerrar_cuenta();
            break;
        case 0:
            // Salir del programa
            printf("Gracias por utilizar nuestro servicio.\n");
            break;
        default:
            printf("Opción no válida. Por favor, seleccione una opción del menú.\n");
            break;
    }
} while(opcion != 0);

return 0;
}