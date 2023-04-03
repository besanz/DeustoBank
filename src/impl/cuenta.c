#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../dec/cuenta.h"
#include "../dec/db.h"

CuentaBancaria* crear_cuenta(Cliente* titular) {
    return db_crear_cuenta(titular);
}

void mostrar_informacion_cuenta(int numero_cuenta) {
    CuentaBancaria* cuenta = db_buscar_cuenta_por_numero(numero_cuenta);
    if (cuenta) {
        printf("Numero de cuenta: %d\n", cuenta->numeroCuenta);
        printf("Saldo: %.2f\n", cuenta->saldo);
        printf("Titular: %s %s\n", cuenta->cliente->nombre, cuenta->cliente->apellido);
    } else {
        printf("No se encontro la cuenta con el numero %d\n", numero_cuenta);
    }
}

void depositar_dinero(int numero_cuenta, float monto) {
    db_depositar_dinero(numero_cuenta, monto);
}

void retirar_dinero(int numero_cuenta, float monto) {
    db_retirar_dinero(numero_cuenta, monto);
}

void transferir_dinero(int cuenta_origen, int cuenta_destino, float monto) {
    db_transferir_dinero(cuenta_origen, cuenta_destino, monto);
}

void cerrar_cuenta(int numero_cuenta) {
    db_cerrar_cuenta(numero_cuenta);
}

CuentaBancaria* buscar_cuenta_por_numero(int numero_cuenta) {
    return db_buscar_cuenta_por_numero(numero_cuenta);
}
