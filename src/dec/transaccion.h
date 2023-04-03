#ifndef TRANSACCION_H
#define TRANSACCION_H

#include "../struct.h"

typedef struct TransaccionNodo {
    Transaccion transaccion;
    struct TransaccionNodo* siguiente;
} TransaccionNodo;

void registrar_transaccion(Transaccion* transaccion);
Transaccion* listar_transacciones(int numero_cuenta);
void eliminar_transaccion(int id_transaccion);
Informe* mostrar_informe_financiero(int numero_cuenta);

TransaccionNodo* crear_nodo_transaccion(Transaccion* transaccion);


#endif // TRANSACCION_H
