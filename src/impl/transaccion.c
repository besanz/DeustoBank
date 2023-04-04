#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <time.h>
#include "../dec/transaccion.h"
#include "../dec/db.h"

void registrar_transaccion(Transaccion* transaccion){
    if (transaccion == NULL) {
        printf("Error: La transaccion no puede ser NULL.\n");
        return;
    }
    db_registrar_transaccion(transaccion);
}

Transaccion *listar_transacciones(const char* numero_cuenta, int *num_transacciones) {
    return db_listar_transacciones(numero_cuenta, num_transacciones);
}

void eliminar_transaccion(int id_transaccion){
    db_eliminar_transaccion(id_transaccion);
}

Informe *mostrar_informe_financiero(const char* numero_cuenta) {
    char str_numero_cuenta[32];
    snprintf(str_numero_cuenta, sizeof(str_numero_cuenta), "%d", numero_cuenta);
    return db_mostrar_informe_financiero(str_numero_cuenta);
}

void realizar_transferencia(int cliente_id_origen) {
    char usuario_destino[50];
    float cantidad;
    Cliente *cliente_destino;
    Cliente *cliente_origen;
    CuentaBancaria *cuenta_bancaria_origen = malloc(sizeof(CuentaBancaria));
    CuentaBancaria *cuenta_bancaria_destino = malloc(sizeof(CuentaBancaria));
    char confirmacion;

    printf("Introduce el nombre de usuario destino: ");
    scanf("%s", usuario_destino);
    int idUser = db_obtener_usuarioID(usuario_destino);
    printf("imprime\n");

    cliente_destino = db_buscar_cliente_por_usuarioID(idUser);
    printf("imprime 2\n ");
    if (cliente_destino == NULL) {
        printf("Usuario destino no encontrado.\n");
        return;
    }

    cliente_origen = db_buscar_cliente_por_id(cliente_id_origen);
    printf("imprime 3\n ");//SÍ LLEGA AQUÍ
    cuenta_bancaria_origen = db_buscar_cuenta_por_cliente(cliente_origen->clienteID);
    cuenta_bancaria_destino = db_buscar_cuenta_por_cliente(cliente_destino->clienteID);
    printf("imprime 4\n "); //NO LLEGA AQUÍ
    if (cuenta_bancaria_origen == NULL || cuenta_bancaria_destino == NULL) {
        printf("Error al obtener cuentas bancarias.\n");
        return;
    }

    printf("Introduce la cantidad a transferir: ");
    scanf("%f", &cantidad);

    if (cantidad <= 0 || cantidad > cuenta_bancaria_origen->saldo) {
        printf("Cantidad invalida. La transferencia no puede realizarse.\n");
        return;
    }

    printf("Informacion de las cuentas:\n\n");
    printf("Cuenta origen:\n");
    printf("Nombre: %s %s\n", cliente_origen->nombre, cliente_origen->apellido);
    printf("Numero de cuenta: %s\n", cuenta_bancaria_origen->numeroCuenta);

    printf("\nCuenta destino:\n");
    printf("Nombre: %s %s\n", cliente_destino->nombre, cliente_destino->apellido);
    printf("Numero de cuenta: %s\n", cuenta_bancaria_destino->numeroCuenta);

    printf("\nCantidad a transferir: %.2f\n", cantidad);

    printf("¿Desea confirmar la transferencia? (S/N): ");
    scanf(" %c", &confirmacion);

    if (toupper(confirmacion) != 'S') {
        printf("Transferencia cancelada.\n");
        return;
    }

    db_transferir_dinero(cuenta_bancaria_origen->numeroCuenta, cuenta_bancaria_destino->numeroCuenta, cantidad);

    printf("Transferencia realizada con exito.\n");
}


