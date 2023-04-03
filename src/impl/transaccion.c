#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "../dec/transaccion.h"
#include "../dec/db.h"

void registrar_transaccion(Transaccion* transaccion){
    if (transaccion == NULL) {
        printf("Error: La transacción no puede ser NULL.\n");
        return;
    }
    db_registrar_transaccion(transaccion);
}

Transaccion* listar_transacciones(int numero_cuenta){
    return db_listar_transacciones(numero_cuenta);
}

void eliminar_transaccion(int id_transaccion){
    db_eliminar_transaccion(id_transaccion);
}

Informe* mostrar_informe_financiero(int numero_cuenta){
    return db_mostrar_informe_financiero(numero_cuenta);
}
