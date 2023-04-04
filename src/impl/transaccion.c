#include <stdlib.h>
#include <stdio.h>
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
